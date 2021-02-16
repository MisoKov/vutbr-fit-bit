<?php
/**
 * Project: Analyzátor kódu v IPPcode20
 * @file parse.php
 * @brief Hlavna trieda scriptu, vykonava lexikalnu a syntakticku analyzu kodu
 *
 * @author Michal Koval <xkoval17@stud.fit.vutbr.cz>
 */

    // the main class of parser.php, parses input does the lexical and syntax analysis
    class Parser {
        private $line;
        
        // checks the .IPPcode20 header, skips empty lines and comments
        // $xmldoc - XMLDoc object
        // $stats - Stats object
        public function headerCheck($xmldoc, $stats){
            $this->firstLineCheck($stats);
            if (preg_match('/.IPPcode20/i', $this->line)){
                $split = preg_split('/.IPPcode20/i', $this->line,2);
                if (strlen($split[0]) > 0 && !ctype_space($split[0])){
                    fprintf(STDERR, "Error: Znaky pred hlavickou\n");
                    throw new Exception("Parser error: invalid header", Errors::ERROR_HEADER);
                }
                if (strlen($split[1]) > 0){
                    if (trim($split[1])[0] != "#"){
                        fprintf(STDERR, "Error: Lexemy za hlavickou .IPPcode20\n");
                        throw new Exception("Parser error: invalid header", Errors::ERROR_HEADER);
                    } else {
                        $stats->addComment();
                    }
                }
            } else {
                fprintf(STDERR, "Error: Chybajuca hlavicka .IPPcode20\n");
                throw new Exception("Parser error: invalid header", Errors::ERROR_HEADER);
            }
        }
        
        // reads lines from stdin until it reads something that is not comment or empty line
        // $stats - Stats object
        private function firstLineCheck($stats){
            $this->line = trim(fgets(STDIN));
            if (strlen($this->line) <= 0){
                if (feof(STDIN)){
                    fprintf(STDERR, "Error: Chybajuca hlavicka .IPPcode20\n");
                    throw new Exception("Parser error: invalid header", Errors::ERROR_HEADER);
                } else {
                    $this->firstLineCheck($stats);
                    return;
                }
            }
            if ($this->line[0] == "#"){
                $stats->addComment();
                $this->firstLineCheck($stats);
                return;
            }
        }
        
        // main function of Parser
        // parses the input and calls functions upon it to make lex and syn check
        // $xmldoc - XMLDoc object
        // $stats - Stats object
        public function analysis($xmldoc, $stats){
            // citanie vstupenho suboru do konca
            while ($this->line = fgets(STDIN)){
                $this->line = trim($this->line);
                // vynechanie prazdnych riadkov
                if (strlen($this->line) <= 0){
                    continue;
                }
                // rozdelenie podla komentarov
                $split = preg_split('/#/', $this->line, 2);
                if (count($split) > 1){
                    $stats->addComment();
                    if (strlen($split[0]) == 0){
                        continue;
                    }
                }
                $stats->addLoc();
                $splitLine = preg_split('/\s+/', $split[0], 4);
                // rozpoznanie instrkcie
                $firstWord = strtolower($splitLine[0]);
                $args = array();
                switch($firstWord){
                    // instrukcie bez parametrov
                    case "return":
                        $stats->addJump();
                    case "createframe":
                    case "pushframe":
                    case "popframe":
                    case "break":
                        // kontrola ziadnych dalsich arguemtov
                        if (isset($splitLine[1])){
                            if (strlen($splitLine[1]) > 0){
                                // syntax error
                                throw new Exception("Syntax error", Errors::ERROR_LEXSYN);
                            }
                        }
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),null);
                        break;
                    // instrukcia - label
                    case "label":
                        // zvlast kvoli zapocitaniu do stats
                        // kontrola poctu parametrov
                        if (count($splitLine) < 2){
                            throw new Exception("Analysis error: missing variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola labelu
                        if (!$this->labelCheck($splitLine[1])){
                            throw new Exception("Analysis error: invalid label", Errors::ERROR_LEXSYN);
                        }
                        // kontrola ziadnych dalsich arguemtov
                        if (isset($splitLine[2])){
                            if (strlen($splitLine[2]) > 0){
                                throw new Exception("Syntax error",Errors::ERROR_LEXSYN);
                            }
                        }
                        // kontrola unikatnosti a priratanie k statistike
                        $stats->addLabel($splitLine[1]);
                        $args[0] = array("type"=>"label","value"=>$splitLine[1]);
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),$args);
                        break;
                    case "jump":
                    case "call":
                        // kontrola poctu parametrov
                        if (count($splitLine) < 2){
                            throw new Exception("Analysis error: missing variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola labelu
                        if (!$this->labelCheck($splitLine[1])){
                            throw new Exception("Analysis error: invalid label", Errors::ERROR_LEXSYN);
                        }
                        // kontrola ziadnych dalsich arguemtov
                        if (isset($splitLine[2])){
                            if (strlen($splitLine[2]) > 0){
                                throw new Exception("Syntax error",Errors::ERROR_LEXSYN);
                            }
                        }
                        $stats->addJump();
                        $args[0] = array("type"=>"label","value"=>$splitLine[1]);
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),$args);
                        break;
                    // instrukcia - symbol
                    case "pushs":
                    case "exit":
                    case "dprint":
                    case "write":
                        // kontrola poctu parametrov
                        if (count($splitLine) < 2){
                            throw new Exception("Analysis error: missing variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola symbolu
                        if (!$this->symbolCheck($splitLine[1])){
                            throw new Exception("Analysis error: invalid symbol", Errors::ERROR_LEXSYN);
                        }
                        // kontrola komentu za prikazom
                        if (isset($splitLine[2])){
                            if (strlen($splitLine[2]) > 0){
                                if ($splitLine[2][0] == '#'){
                                    $stats->addComment();
                                } else {
                                    throw new Exception("Syntax error",Errors::ERROR_LEXSYN);
                                }
                            }
                        }
                        $symb = $this->processSymbol($splitLine[1]);
                        $args[0] = array("type"=>$symb["type"],"value"=>$symb["value"]);
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),$args);
                        break;
                    // instrukcia - var
                    case "pops":
                    case "defvar":
                        // kontrola poctu parametrov
                        if (count($splitLine) < 2){
                            throw new Exception("Analysis error: missing variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola premennej
                        if (!$this->varCheck($splitLine[1])){
                            throw new Exception("Analysis error: invalid variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola ziadnych dalsich arguemtov
                        if (isset($splitLine[2])){
                            if (strlen($splitLine[2]) > 0){
                                throw new Exception("Syntax error",Errors::ERROR_LEXSYN);
                            }
                        }
                        $args[0] = array("type"=>"var","value"=>$splitLine[1]);
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),$args);
                        break;
                    // instrukcia - var symbol
                    case "not":
                    case "move":
                    case "int2char":
                    case "strlen":
                    case "type":
                        // kontrola poctu parametrov
                        if (count($splitLine) < 3){
                            throw new Exception("Analysis error: missing variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola premennej
                        if (!$this->varCheck($splitLine[1])){
                            throw new Exception("Analysis error: invalid variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola symbolu
                        if (!$this->symbolCheck($splitLine[2])){
                            throw new Exception("Analysis error: invalid symbol", Errors::ERROR_LEXSYN);
                        }
                        // kontrola ziadnych dalsich argumentov
                        if (isset($splitLine[3])){
                            if (strlen($splitLine[3]) > 0){
                                throw new Exception("Syntax error",Errors::ERROR_LEXSYN);
                            }
                        }
                        $args[0] = array("type"=>"var","value"=>$splitLine[1]);
                        $symb = $this->processSymbol($splitLine[2]);
                        $args[1] = array("type"=>$symb["type"],"value"=>$symb["value"]);
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),$args);
                        break;
                    // instrukcia - var sym sym
                    case "concat":
                    case "getchar":
                    case "setchar":
                    case "stri2int":
                    case "and":
                    case "or":
                    case "lt":
                    case "gt":
                    case "eq":
                    case "add":
                    case "sub":
                    case "idiv":
                    case "mul":
                        // kontrola poctu parametrov
                        if (count($splitLine) < 4){
                            throw new Exception("Analysis error: missing variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola premennej
                        if (!$this->varCheck($splitLine[1])){ //ofset
                            throw new Exception("Analysis error: invalid variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola symbolov
                        if (!$this->symbolCheck($splitLine[2])){
                            throw new Exception("Analysis error: invalid symbol",Errors::ERROR_LEXSYN);
                        }
                        if (!$this->symbolCheck($splitLine[3])){ //ofset
                            throw new Exception("Analysis error: invalid symbol",Errors::ERROR_LEXSYN);
                        }
                        // kontrola ziadnych dalsich argumetov
                        if (isset($splitLine[4])){
                            if (strlen($splitLine[4]) > 0){
                                throw new Exception("Syntax error",Errors::ERROR_LEXSYN);
                            }
                        }
                        $args[0] = array("type"=>"var","value"=>$splitLine[1]);
                        $symb = $this->processSymbol($splitLine[2]);
                        $args[1] = array("type"=>$symb["type"],"value"=>$symb["value"]);
                        $symb = $this->processSymbol($splitLine[3]);
                        $args[2] = array("type"=>$symb["type"],"value"=>$symb["value"]);
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),$args);
                        break;
                    // instrukcia - label symb symb
                    case "jumpifeq":
                    case "jumpifneq":
                        // kontrola poctu parametrov
                        if (count($splitLine) < 4){
                            throw new Exception("Analysis error: missing variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola labelu
                        if (!$this->labelCheck($splitLine[1])){ // offsety aj tu 
                            throw new Exception("Analysis error: invalid variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola symbolov
                        if (!$this->symbolCheck($splitLine[2])){
                            throw new Exception("Analysis error: invalid symbol",Errors::ERROR_LEXSYN);
                        }
                        if (!$this->symbolCheck($splitLine[3])){
                            throw new Exception("Analysis error: invalid symbol",Errors::ERROR_LEXSYN);
                        }
                        // kontrola ziadnych dalsich argumetov
                        if (isset($splitLine[4])){
                            if (strlen($splitLine[4]) > 0){
                                throw new Exception("Syntax error",Errors::ERROR_LEXSYN);
                            }
                        }
                        $stats->addJump();
                        $args[0] = array("type"=>"label","value"=>$splitLine[1]);
                        $symb = $this->processSymbol($splitLine[2]);
                        $args[1] = array("type"=>$symb["type"],"value"=>$symb["value"]);
                        $symb = $this->processSymbol($splitLine[3]);
                        $args[2] = array("type"=>$symb["type"],"value"=>$symb["value"]);
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),$args);
                        break;
                    // instrukcia - var type
                    case "read":
                        // kontrola poctu parametrov
                        if (count($splitLine) < 3){
                            throw new Exception("Analysis error: missing variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola premennej
                        if (!$this->varCheck($splitLine[1])){
                            throw new Exception("Analysis error: invalid variable", Errors::ERROR_LEXSYN);
                        }
                        // kontrola typu
                        if (!$this->typeCheck($splitLine[2])){
                            throw new Exception("Analysis error: invalid type for read instruction", Errors::ERROR_LEXSYN);
                        }
                        // kontrola ziadnych dalsich arguemtov
                        if (isset($splitLine[3])){
                            if (strlen($splitLine[3]) > 0){
                                throw new Exception("Syntax error",Errors::ERROR_LEXSYN);
                            }
                        }
                        $args[0] = array("type"=>"var","value"=>$splitLine[1]);
                        $args[1] = array("type"=>"type","value"=>$splitLine[2]);
                        $xmldoc->addInstruction(strtoupper($splitLine[0]),$args);
                        break;
                    default:
                        // neznama instrukcia
                        $stats->subLoc();
                        throw new Exception("Analysis error: opcode\n", Errors::ERROR_OPCODE);
                }
            }
        }
        
        // determines the type of symbol
        // $symbol - string constaining the symbol
        private function processSymbol($symbol){
            $split = preg_split('/@/', $symbol, 2);
            if (strlen($split[0]) == 2){
                return array("type"=>"var","value"=>$symbol);
            } else {
                return array("type"=>$split[0],"value"=>$split[1]);
            }
        }
        
        // checks the lexcal validity of label name
        // $label - string containing label name
        private function labelCheck($label){
            $label = trim($label);
            $len = strlen($label);
            for ($i = 0; $i < $len; $i++){
                if ($i == 0){
                    // first character cannot be number
                    if ($label[0] == '_' || $label[0] == '-' || 
                        $label[0] == '$' || $label[0] == '%' || 
                        $label[0] == '*' || $label[0] == '!' || 
                        $label[0] == '?' || $label[0] == '&'){
                        continue;
                    }
                    if (ctype_alpha($label[$i])){
                        continue;
                    } else {
                        return false;
                    }
                } else {
                    if ($label[$i] == '_' || $label[$i] == '-' || 
                        $label[$i] == '$' || $label[$i] == '%' || 
                        $label[$i] == '*' || $label[$i] == '!' || 
                        $label[$i] == '?' || $label[$i] == '&'){
                        continue;
                    } else if (ctype_alnum($label[$i])){
                        continue;
                    } else {
                        return false;
                    }
                }
            }
            return true;
        }
        
        // lexical check of variable
        // $var - string containing variable 
        private function varCheck($var){
            $var = trim($var);
            $splitVar = preg_split('/@/', $var, 2);
            if (count($splitVar) == 2){
                if ($splitVar[0] == "GF" || $splitVar[0] == "LF"||
                    $splitVar[0] == "TF"){
                    // na nazov premennej sa vztahuju rovnake pravidla ako na label
                    return $this->labelCheck($splitVar[1]);
                }
            }
            return false;
        }
        
        // checks for validity of type
        // $type - string containing type
        private function typeCheck($type){
            $type = trim($type);
            if ($type == "string" || $type == "int" || $type == "bool"){
                return true;
            } else {
                return false;
            }
        }
        
        // checks the lexical validity of symbol
        // $symbol - string containing symbol
        private function symbolCheck($symbol){
            $split = preg_split('/@/', $symbol, 2);
            if (count($split) != 2){
                return false;
            }
            if (strlen($split[0]) == 2){
                return $this->varCheck($symbol);
            }
            if ($split[0] == "string"){
                return $this->stringCheck($split[1]);
            } else if ($split[0] == "int"){
                return $this->intCheck($split[1]);
            } else if ($split[0] == "bool"){
                if ($split[1] == "true" || $split[1] == "false"){
                    return true;
                } else {
                    return false;
                }
            } else if ($split[0] == "nil"){
                if ($split[1] == "nil"){
                    return true;
                } else {
                    return false;
                }
            } else {
                // nevalidny typ symbolu
                return false;
            }
        }
        
        // checks lexical validity of integer
        // $int - string containing integer
        private function intCheck($int){
            $int = trim($int);
            $len = strlen($int);
            if ($len == 0){
                return false;
            }
            if ($int[0] == '-' || $int[0] == '+' || ($int[0] >= '0' && $int[0] <='9')){
                for ($i = 1; $i < $len; $i++){
                    if (!($int[$i] >= '0' && $int[$i] <='9')){
                        return false;
                    }
                }
            }
            return true;
        }
        
        // checks lexical validity of string
        // $string - string containing string
        private function stringCheck($string){
            $string = trim($string);
            $len = strlen($string);
            if ($len == 0){
                return true;
            }
            for ($i = 0; $i < $len; $i++){
                if ($string[$i] >= chr(0) && $string[$i] <=chr(32)){
                    // znaky s acii val od 0 do 32
                    return false;
                } else if ($string[$i] == '\\'){
                    // escape sequence
                    if ($i + 3 < $len){
                        if (($string[$i+1] >= '0' && $string[$i+1] <= '9') &&
                            ($string[$i+2] >= '0' && $string[$i+2] <= '9') &&
                            ($string[$i+3] >= '0' && $string[$i+3] <= '9')){
                                $i += 2;
                                continue;
                            } else {
                                return false;
                            }
                    } else {
                        return false;
                    }
                } else if ($string[$i] == 35){
                    // #
                    return false;
                }
            }
            return true;
        }
    }
?>
