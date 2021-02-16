Implementační dokumentace k 1. úloze do IPP 2019/2020
Jméno a příjmení: Michal Koval
Login: xkoval17
___
## 1 Implementation
The main file is parser.php. The program was designed in object oriented way. From main script are called the main methods of classes specified to solve the subproblems of project. My implementation does not use the finite state machine control flow, instead functions are just called directly which then call other function for smaller problems. In case of valid input program prints the XML representation of input code and returns 0, otherwise returns one of the error codes defined in Errors.php file. The main script first calls the method to proccess arguments, then to perform the lexical and syntax analysis upon the input code and finally prints resulting XML representation of code.
### 1.1 Processing arguments
Class Arguments implemented in Arguments.php file. Checks and proccesses arguments. Method `checkArguments($stats)` proccesses all the validation of arguments. Checks the correct number of arguments. Checks the duplicity of stats file and sets the stats file in $stats object. Checks the incorrect combinations of arguments. In case of argurment error throws exception with code 10.
### 1.2 Parsers - analysis
Implemented as class Parser in Parser.php file. Firstly header is checked with method `headerCheck($xmldoc, $stats)`. Method `analysis($xmldoc, $stats)` reads the input code from standard input stream until the end of file. Input is parsed the opcode is identified and then the syntax validation is performed with current instruction. Submethods are called to execute the lexical validation upon parameters of instruction.
### 1.3 XML representation
Class XMLDoc implemented in XMLDoc.php. Uses DomDocument library to compose the XML document representing the code. Only has 2 methods. `addInstruction($instruction, $args)` adds passed instruction and its arguments to the final XML code which is stored internally in $output variable. Variable $instructionIndex is used to track the order of instructions. Function `printDoc()` prints the XML code to the standard output.
___
## 2 Extensions
The only available extension for this part of project was STATP which was implemented in my solving.
### 2.1 STATP
Implemented in file Stats.php as class Stats. Provides variables used as counters and methods to increase the counts of comments, instructions, jump and unique labels in code. The methods are then called during parsing when the tokens are indentified and validated. Prints the results to the file passed with option --stats. Stats are printed in the order that they ware passed as arguments to the parser.php script. Only prints statistics if the input code was valid. 
