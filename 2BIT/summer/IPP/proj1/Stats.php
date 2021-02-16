<?php
/**
 * Project: Analyzátor kódu v IPPcode20
 * @file Stats.php
 * @brief Trieda stats sluzi na zber a ulozenie statisky o kode
 *
 * @author Michal Koval <xkoval17@stud.fit.vutbr.cz>
 */
 
    class Stats {
        private $locCount;
        private $commentCount;
        private $labelCount;
        private $jumpCount;
        private $labels;
        
        private $allowPrint;
        
        private $filename;
        
        public function __construct() {
        $this->locCount = 0;
        $this->commentCount = 0;
        $this->jumpCount = 0;
        $this->labelCount = 0;
        $this->labels = array();
        
        $this->allowPrint = false;
        }
        
        // sets file name for output file
        // $name - name of output file
        public function setFileName($name) {
            $this->filename = $name;
        }
        
        public function addLoc() {
            $this->locCount++;
        }
        
        public function subLoc() {
            $this->locCount--;
        }
        
        public function addComment() {
            $this->commentCount++;
        }
        
        public function addJump() {
            $this->jumpCount++;
        }
        
        // if uniques labelName is passed increases label counter
        // $labelName - string containing name of label
        public function addLabel($labelName) {
            if (!in_array($labelName, $this->labels)){
                $this->labelCount++;
                array_push($this->labels,$labelName);
            }
        }
        
        public function allowPrint() {
            $this->allowPrint = true;
        }
        
        // prints final results in correct order to the output file
        public function printStats() {
            global $argv;
            $text = "";
            if ($this->allowPrint) {
                foreach ($argv as $arg) {
                    if ($arg == '--loc')
                        $text = $text.$this->locCount."\n";
                    else if ($arg == '--comments')
                        $text = $text.$this->commentCount."\n";
                    else if ($arg == '--labels')
                        $text = $text.$this->labelCount."\n";
                    else if ($arg == '--jumps')
                        $text = $text.$this->jumpCount."\n";
                }
                file_put_contents($this->filename, rtrim($text));
            }
        }
    }
?>
