<?php
/**
 * Project: Analyzátor kódu v IPPcode20
 * @file Arguments.php
 * @brief Trieda Arguments sluzi na spracovanie vstupnych argumentov
 *
 * @author Michal Koval <xkoval17@stud.fit.vutbr.cz>
 */
 
     class Arguments {
        private $options = array("help", "stats:", "loc", "labels", "jumps", "comments");
        private const HELP = "Skript typu filtr nacte ze standardniho vstupu zdrojovy kód\n".
                              "v IPP-code20, zkontroluje lexikalni a syntaktickou spravnost\n".
                              "kodu a vypise na standardni vystup XML reprezentaci programu.\n".
                              "Parametry:\n".
                              "--help - vypise na standardni vystup napovedu skriptu, solo pouzitie\n".
                              "--stats=file - sluzi na zadanie file, do ktoreho sa vypisu agregovane\n".
                              "               statistiky, moze byt zadany len jeden file\n".
                              "--loc - vypise do statistik pocet riadkov s instrukciami\n".
                              "--comments - vypise do statistik pocet riadkov s komentarmi\n".
                              "--labels - vypise do statistik pocet definovanych navesti\n".
                              "--jumps - vypise do statistik pocet instrukci pre skoky\n";
        
        // proccesses the arguments and checks their validity
        // $stats - Stats object, needed for to set output file for stats
        public function checkArguments ($stats){
            // getting the options
            $opts = getopt(null, $this->options);
            // checking validity of arguments
            $optsCount = 0;
            global $argc;
            foreach ($opts as $opt){
                if (is_countable($opt)){
                    $optsCount += count($opt);
                } else {
                    $optsCount++;
                }
            }
            // check for correct numbers of arguments - validity of arguments
            if ($optsCount != $argc-1){
                fprintf(STDERR, "Error: Incorrect arguments passed to the program, use --help.\n");
                throw new Exception("Incorrect arguments", Errors::ERROR_ARGS);
            }
            // check for only one stats file
            if (isset($opts["stats"]) && is_countable($opts["stats"])){
                fprintf(STDERR, "Error: 2 or more output files entered for stats option, use --help.\n");
                throw new Exception("Incorrect arguments - more stats files", Errors::ERROR_ARGS);
            }
            // setting filename for stats
            if (isset($opts["stats"])){
                $stats->setFileName($opts["stats"]);
            }
            // checking help
            if (isset($opts["help"])){
                if ($optsCount == 1){
                    echo self::HELP;
                    throw new Exception("Help print", Errors::OK);
                } else { 
                    fprintf(STDERR, "Error: Help option can be used only alone, use --help.\n");
                    throw new Exception("Incorrect arguments - help", Errors::ERROR_ARGS);
                }
            }
            // setting the allowPrint
            if (isset($opts["loc"]) || isset($opts["labels"]) ||
                isset($opts["jumps"]) || isset($opts["comments"]))
                // check for no stats set
                if (!isset($opts["stats"])){
                    fprintf(STDERR, "Error: Stats file must be set if stats options are used, use --help.\n");
                    throw new Exception("Incorrect arguments - stats file missing", Errors::ERROR_ARGS);
                } else 
                    $stats->allowPrint();
        }
    }
?>
