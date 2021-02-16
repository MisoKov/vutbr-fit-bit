<?php
/**
 * Project: Testovací rámec pre projekt IPP 2020
 * @file test.php
 * @brief Testovaci script pre analyzator aj interpreter jazyka IPPcode20
 *
 * @author Michal Koval <xkoval17@stud.fit.vutbr.cz>
 */
 
ini_set('display_errors', 'stderr');
require_once(__DIR__.'/Errors.php');
// ----------------- Main ----------------------
try{
	$test = new Testing();
	$args = new Arguments();
	$args->processArgs($test);
	$test->startTests();
} catch(Exception $e){
	exit($e->getCode());
}
exit(Errors::OK);

// -------------- Functions ------------------------
function fileExists($file){
    $path = realpath($file);
    if($path !== false && file_exists($path))
    {
        return true;
    }
    // Path/file does not exist
    return false;
}
// -------------- Classes ------------------------
// class for argument proccessing
class Arguments{
	private $options = array("help", "parse-script:", "recursive", "parse-only", "int-only", "jexamxml:", "int-script:", "directory:");
	
	// processes and checks the validity of arguments
	// $test - Testing object
	public function processArgs($test){
		global $argv;
		global $argc;
		// getting the options
        $opts = getopt(null, $this->options);
        // checking validity of arguments
        $optsCount = 0;
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
        for ($i=1; $i<$argc; $i++){
			if (strpos($argv[$i], "=")){
				$split = preg_split('/=/', $argv[$i], 2);
				if ($split[0] == "--help" || $split[0] == "--recursive" ||
					$split[0] == "--parse-only" || $split[0] == "--int-only"){
					fprintf(STDERR, "Error: Incorrect arguments passed to the program, use --help.\n");
					throw new Exception("Argument error", Errors::ERROR_ARGS);
				}
			}
		}
		// checking help
        if (isset($opts["help"])){
            if ($optsCount == 1 && $argc == 2){
                $this->printHelp();
                throw new Exception("Help print", Errors::OK);
            } else { 
                fprintf(STDERR, "Error: Help option can be used only alone, use --help.\n");
                throw new Exception("Incorrect arguments - help", Errors::ERROR_ARGS);
            }
        }
        // checking for duplicity
        $this->duplicityCheck($opts, "parse-script");
        $this->duplicityCheck($opts, "jexamxml");
        $this->duplicityCheck($opts, "int-script");
        $this->duplicityCheck($opts, "directory");
      
        // parser/interpreter only check
        if (isset($opts["parse-only"])){
        	if (isset($opts["int-only"]) || isset($opts["int-script"])){
        		fprintf(STDERR, "Error arguments: parser-only and int-only/int-script used at same time, use --help.\n");
           		throw new Exception("Incorrect arguments - duplicity", Errors::ERROR_ARGS);
        	} else {
        	    $test->setMode(Testing::MODE_PARSER);
        	}
        } else if (isset($opts["int-only"])){
        	if (isset($opts["parse-only"] )|| isset($opts["parse-script"])){
        		fprintf(STDERR, "Error arguments: int-only and parser-only/parser-script used at same time, use --help.\n");
           		throw new Exception("Incorrect arguments - duplicity", Errors::ERROR_ARGS);
        	} else {
        	    $test->setMode(Testing::MODE_INT);
        	}
        }
        // setting the testing parameters
        $this->pathCheck($opts, "directory", $test);
        $this->pathCheck($opts, "jexamxml", $test);
        $this->pathCheck($opts, "parse-script", $test);
        $this->pathCheck($opts, "int-script", $test);
        
        if (isset($opts["recursive"])){
            $test->setRecursion();
        }
	}
    
    // cheks if the paths entered in arguments exists are are valid
    private function pathCheck($opts, $opt, $test){
        if (isset($opts[$opt])){
            if (fileExists($opts[$opt])){
            	if ($opt == "directory"){
            		$test->setDirectory($opts[$opt]);
            	} else if ($opt == "jexamxml"){
            		if (fileExists($opts[$opt]."/jexamxml.jar")){
            			$test->setJexamxml($opts[$opt]."/jexamxml.jar");
            		} else {
            			fprintf(STDERR, "Error arguments: folder entered for jexamxml does not content jexamxml.jar, use --help.\n");
           				throw new Exception("Incorrect arguments - jexamxml not found", Errors::ERROR_INPUT);
            		}
            	} else if ($opt == "parse-script"){
            		$test->setParser($opts[$opt]);
            	} else if ($opt == "int-script"){
            		$test->setInterpret($opts[$opt]);
            	}
            } else {
                fprintf(STDERR, "Error arguments: non-existent ".$opt." path, use --help.\n");
           		throw new Exception("Incorrect arguments - ".$opt." path", Errors::ERROR_INPUT);
            }
        }
    }
    
    // checks if arguemt was passed to the program more than once
    private function duplicityCheck($opts, $opt){
        if (isset($opts[$opt]) && is_countable($opts[$opt])){
  	      	fprintf(STDERR, "Error: 2 or more ".$opt." options entered, use --help.\n");
            throw new Exception("Incorrect arguments - duplicity", Errors::ERROR_ARGS);
        }
    }

    // prints help
	private function printHelp()
	{
		echo "  php7.4 test.php [--help] [--recursive] [--parse-script=] [--int-script=] [--directory=] [--parse-only] [--int-only] [--jexamxml=]\n";
		echo "     --help	     - Zobrazi napovedu\n";
		echo "     --recursive     - Recursivni prohledavani adresaru\n";
		echo "     --parse-script  - Parsovaci script, defaultne parse.php\n";
		echo "     --int-script    - Interpretovaci script, defaultne interpret.py\n";
		echo "     --directory     - Adresar s testmi, defaultne aktualni adresar\n";
		echo "     --parse-only    - Bude testován pouze skript pro analýzu zdrojového kódu v IPPcode20\n";
		echo "     --int-only      - Bude testován pouze skript pro interpret XML reprezentace kódu v IPPcode20\n";
		echo "     --jexamxml      - Soubor s JAR balíčkem JExamXML, defaultne /pub/courses/ipp/jexamxml/jexamxml.jar\n";
	}
}

// main testing class
class Testing{
	private $parser;
	private $interpret;
	private $jexamxml;
	private $directory;
	private $recursive;
	private $mode;
	private $results;
	private $passed;
	private $rc;
	private $diff;
	
	// mody testovavia
	const MODE_PARSER = 0;
	const MODE_INT = 1;
	const MODE_BOTH = 2;
	
    // vysledky testu
	const PASSED = 0;
	const FAIL_RC = 1;
	const FAIL_DIFF = 2;
	
	public function __construct(){
		$this->parser = "parse.php";
		$this->interpret = "interpret.py";
		$this->jexamxml = "/pub/courses/ipp/jexamxml/jexamxml.jar";
		$this->directory = realpath("./");
		$this->recursive = false;
		$this->mode = self::MODE_BOTH;
		$this->results = array();
		$this->passed = 0;
		$this->rc = 0;
		$this->diff = 0;
	}
	
	public function setParser($parser){
		$this->parser = $parser;
	}
	public function setInterpret($interpret){
		$this->interpret = $interpret;
	}
	public function setJexamxml($xml){
		$this->jexamxml = $xml;
	}
	public function setDirectory($dir){
		$this->directory = $dir;
	}
	public function setRecursion(){
		$this->recursive = true;
	}
	public function setMode($mode){
		$this->mode = $mode;
	}
	
	// runs test according to set mode
	// $testname - name of .src file for testcase
	// $dir - directory containing the .src testcase
	private function runTest($testname, $dir){
	    if ($this->mode == self::MODE_PARSER){
	        return $this->runParserOnlyTest($testname, $dir);
	    } else if ($this->mode == self::MODE_INT){
	        return $this->runIntOnlyTest($testname, $dir);
	    } else if ($this->mode == self::MODE_BOTH){
	        return $this->runBothTest($testname, $dir);
	    } else {
	        throw new Exception("Internal error",Errors::ERROR_INTERN);
	    }
	}
	
	// scans dir to find .src files and run tests with them and collects results
	// $dir - directory which is to be scanned for tests
	private function scanDir($dir){
		$this->results[$dir]["passed"] = array();
		$this->results[$dir]["total"] = 0;
		$this->results[$dir]["rc"] = array();
		$this->results[$dir]["diff"] = array();
		$folder = scandir($dir);
		foreach($folder as $file){
			if (is_dir($dir."/".$file)){
				if ($this->recursive == true){
					if ($file != "." && $file !=".."){
						$this->scanDir($dir."/".$file);
					}
				}
			} else {
				if(preg_match("/.src$/", $file)){
					$testname = substr($file, 0, -4);
					$this->results[$dir]["total"]++;
					$this->filesCheck($dir."/".$testname);
					$result = $this->runTest($testname, $dir);
					if ($result == self::PASSED){
						array_push($this->results[$dir]["passed"], $dir."/".$file);
						$this->passed++;
					} else if ($result == self::FAIL_RC){
						array_push($this->results[$dir]["rc"], $dir."/".$file);
						$this->rc++;
					} else if ($result == self::FAIL_DIFF){
						array_push($this->results[$dir]["diff"], $dir."/".$file);
						$this->diff++;
					}
				}
			}
		}
	}
	
	// makes checks for needed program for testing and starts the testing
	public function startTests(){
		if ($this->mode == self::MODE_PARSER){
			if (!fileExists($this->jexamxml)){
        		fprintf(STDERR, "Error input: jexamxml not found, use --help.\n");
           		throw new Exception("Jexamxml not found", Errors::ERROR_INPUT);
        	} else if (!fileExists($this->parser)){
        		fprintf(STDERR, "Error input: parser not found, use --help.\n");
           		throw new Exception("Parser not found", Errors::ERROR_INPUT);
        	}
		} else if ($this->mode == self::MODE_INT){
			if (!fileExists($this->interpret)){
        		fprintf(STDERR, "Error input: interpret not found, use --help.\n");
           		throw new Exception("Interpret not found", Errors::ERROR_INPUT);
        	}
		} else {
			if (!fileExists($this->parser)){
        		fprintf(STDERR, "Error input: parser not found, use --help.\n");
           		throw new Exception("Parser not found", Errors::ERROR_INPUT);
        	} else if (!fileExists($this->interpret)){
        		fprintf(STDERR, "Error input: interpret not found, use --help.\n");
           		throw new Exception("Interpret not found", Errors::ERROR_INPUT);
        	}
		}
		$this->scanDir($this->directory);
		$this->genStart();
		$this->genResults();
	}
	
	// runs test for one testing case in mode interpreter only
	// $testname - name of testcase
	// $dir - directorey containing the test
	private function runIntOnlyTest($testname, $dir){
	    exec("python3.8 $this->interpret --source=".$dir."/".$testname.".src --input=".$dir."/".$testname.".in >".$dir."/".$testname.".my_out", $dump, $rc);
		$rc_file = fopen($dir."/".$testname.".rc","r");
		$test_rc = fgets($rc_file);
		fclose($rc_file);
		if ($rc != $test_rc){
			unlink($dir."/".$testname.".my_out");
			return self::FAIL_RC;
		}
		if ($rc == Errors::OK){
			exec("diff ".$dir."/".$testname.".my_out ".$dir."/".$testname.".out", $dump, $rc);
			if ($rc != Errors::OK){
				unlink($dir."/".$testname.".my_out");
				return self::FAIL_DIFF;
			}
		}
		unlink($dir."/".$testname.".my_out");
		return self::PASSED;
	}
	
	// runs test for one testing case in mode parser only
	// $testname - name of testcase
	// $dir - directorey containing the test
	private function runParserOnlyTest($testname, $dir){
	    exec("php7.4 $this->parser <".$dir."/".$testname.".src >".$dir."/".$testname.".your_out", $dump, $rc);
	    $file = fopen($dir."/".$testname.".rc", "r");
	    $rcFile = fgets($file);
	    if ($rc != $rcFile){
	    	unlink($dir."/".$testname.".your_out");
	    	return self::FAIL_RC;
	    }
	    if ($rc != Errors::OK){
	    	unlink($dir."/".$testname.".your_out");
			return self::PASSED;
	    }
		exec("java -jar ".$this->jexamxml." ".$dir."/".$testname.".your_out ".$dir."/".$testname.".out delta.xml /pub/courses/ipp/jexamxml/options", $dump, $diff);
		unlink("delta.xml");
		if($diff != Errors::OK){
			unlink($dir."/".$testname.".your_out");
			if (fileExists($dir."/".$testname.".your_out.log")){
				unlink($dir."/".$testname.".your_out.log");
			}
			return self::FAIL_DIFF;
		}
		unlink($dir."/".$testname.".your_out");
		return self::PASSED;
	}
	
	// runs test for one testing case in mode both
	// $testname - name of testcase
	// $dir - directorey containing the test
	private function runBothTest($testname, $dir){
		exec("php7.4 $this->parser <".$dir."/".$testname.".src >".$dir."/".$testname.".xml", $dump, $rc);
		if ($rc != Errors::OK){
			unlink($dir."/".$testname.".xml");
			return self::FAIL_RC;
		}
		exec("python3.8 $this->interpret --source=".$dir."/".$testname.".xml --input=".$dir."/".$testname.".in >".$dir."/".$testname.".my_out", $output, $rc);
		$rc_file = fopen($dir."/".$testname.".rc","r");
		$test_rc = fgets($rc_file);
		fclose($rc_file);
		if ($rc != $test_rc){
			unlink($dir."/".$testname.".xml");
			unlink($dir."/".$testname.".my_out");
			return self::FAIL_RC;
		}
		if ($rc == Errors::OK){
			exec("diff ".$dir."/".$testname.".my_out ".$dir."/".$testname.".out", $dump, $rc);
			if ($rc != Errors::OK){
				unlink($dir."/".$testname.".my_out");
				unlink($dir."/".$testname.".xml");
				return self::FAIL_DIFF;
			}
		}
		unlink($dir."/".$testname.".my_out");
		unlink($dir."/".$testname.".xml");
		
		return self::PASSED;
	}
	
	// checks if testing files exist, if not creates them properly
	// $srcname - name of .src file
	private function filesCheck($srcname){
	    $this->missingFileCheck($srcname.".in");
	    $this->missingFileCheck($srcname.".out");
	    if ($this->missingFileCheck($srcname.".rc")){
	        $file = fopen($srcname.".rc", "w");
			if($file == false){
				fprintf(STDERR, "Error: couldn`t open ".$srcname.".rc file\n");
				throw new Exception("Error: couldn`t open ".$srcname.".rc file", Errors::ERROR_OUTPUT);
			}
			fwrite($file, "0");
			fclose($file);
	    }
	}
	
	// checks if file exists, if not creates it
	// $file - filename is is to be checked
	private function missingFileCheck($file){
	    if (!file_exists($file)){
			$succ = touch($file);
			if($succ == false){
				fprintf(STDERR, "Error: couldn`t create ".$file." file\n");
				throw new Exception("Error: couldn`t create ".$file." file\n",Errors::ERROR_OUTPUT);
			}
			return true;
	    }
	    return false;
	}
	
	// prints start of the results in html
	private function genStart(){
		echo "<html lang=\"en\">
    <head>
        <meta charset=\"utf-8\">
        <title>IPPcode20 Test Results</title>
        <meta name=\"author\" content=\"Michal Koval\">
        <style type=\"text/css\">
        .table {
            border: 1px solid black;
            width: 700px;
        }
        .table_header {
            border-bottom: 1px solid black;
            background-color: silver;
        }
        .testcase_cell {
            border-bottom: 1px solid gray;
        }
        .testdir {
            border-bottom: 1px solid gray;
            background-color: goldenrod;
        }
        .test_ok_cell {
            border-bottom: 1px solid gray;
            background-color: green;
        }
        .test_rc_cell {
            border-bottom: 1px solid gray;
            background-color: red;
        }
        .test_diff_cell {
            border-bottom: 1px solid gray;
            background-color: crimson;
        }
        </style>
    </head>
    <body>
        <h1>Test Summary</h1>";
	}
	
	// prints results of testing in html
	private function genResults(){
		$totalRC = $this->rc;
		$totalDiff = $this->diff;
		$totalPassed = $this->passed;
		$totalTests = $totalRC + $totalDiff + $totalPassed;
		
		echo "<p><b>
            Total tests: ".$totalTests."<br>
            Passed: ".$totalPassed."<br>
            Failed: ".($totalDiff+$totalRC)."<br>
            Failed-rc: ".$totalRC."<br>
            Failed-output: ".$totalDiff."
        </b></p>
        <table class=\"table\" cellspacing=\"0\">
            <thead>
                <tr>
                    <td class=\"table_header\">
                        Test Case
                    </td>
                    <td class=\"table_header\">
                        Result
                    </td>
                </tr>
            </thead>
            <tbody>";
        foreach ($this->results as $res){
        	if ($res["total"] == 0){
				unset($res);
			} else {
				$dir = array_search($res, $this->results);
				$len = strlen($dir) +1;
				echo "<tr>
                    <td class=\"testdir\">
                        ".$dir."
                    </td>
                    <td class=\"testdir\">
                        ".count($res["passed"])."/".$res["total"]."
                    </td>
                </tr>";
                foreach ($res["passed"] as $passed){
                	echo "<tr>
                    <td class=\"testcase_cell\">
                        ".substr($passed, $len)."
                    </td>
                    <td class=\"test_ok_cell\">
                        OK
                    </td>
                </tr>";
                }
                foreach ($res["rc"] as $rc){
                	echo "<tr>
                    <td class=\"testcase_cell\">
                        ".substr($rc, $len)."
                    </td>
                    <td class=\"test_rc_cell\">
                        Return code
                    </td>
                </tr>";
                }
                foreach ($res["diff"] as $diff){
                	echo "<tr>
                    <td class=\"testcase_cell\">
                        ".substr($diff, $len)."
                    </td>
                    <td class=\"test_diff_cell\">
                        Different output
                    </td>
                </tr>";
                }
			}
        }
        echo " </tbody>
        </table>
    </body>
</html>";
	}
}
?>
