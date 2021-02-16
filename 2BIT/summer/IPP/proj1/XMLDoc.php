<?php
/**
 * Project: Analyzátor kódu v IPPcode20
 * @file XMLDoc.php
 * @brief Trieda XMLDoc sluziaca na generovanie vystupneho kodu v XML
 *
 * @author Michal Koval <xkoval17@stud.fit.vutbr.cz>
 */

    // class is used to form the resulting XML representaion of code
    class XMLDoc{
        private $output;
        private $program;
        private $instructionIndex;
        
        public function __construct(){
            $this->output = new DomDocument("1.0","UTF-8");
            $this->output->formatOutput=true;
            $this->program = $this->output->createElement("program");
            $this->program->setAttribute("language","IPPcode20");
            $this->output->appendChild($this->program);
            $this->instructionIndex = 1;
        }
        
        // adds the instruction and its parameters to the final XML representation of code
        // $instruction - opcode of instruction
        // $args - list containing tuples of type and value of parameters of intruction
        public function addInstruction($instruction,$args){
            $temp = $this->output->createElement("instruction");
            $temp->setAttribute("order",strval($this->instructionIndex));
            $temp->setAttribute("opcode",$instruction);
            
            if ($args != null){
                $argsCount = count($args);
                if ($argsCount >= 1){
                    $arg1 = $this->output->createElement("arg1", htmlspecialchars($args[0]["value"]));
                    $arg1->setAttribute("type", $args[0]["type"]);
                    $temp->appendChild($arg1);
                }
                 if ($argsCount >= 2){
                    $arg2 = $this->output->createElement("arg2", htmlspecialchars($args[1]["value"]));
                    $arg2->setAttribute("type", $args[1]["type"]);
                    $temp->appendChild($arg2);
                }
                if ($argsCount == 3){
                    $arg3 = $this->output->createElement("arg3", htmlspecialchars($args[2]["value"]));
                    $arg3->setAttribute("type", $args[2]["type"]);
                    $temp->appendChild($arg3);
                    
                }
            }
            $this->program->appendChild($temp);
            $this->instructionIndex++;
        }
        
        // prints final XML representaion
        public function printDoc(){
            echo $this->output->saveXML();
        }
    }
?>
