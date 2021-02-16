from modules.errors import ErrorInput, ErrorArgs, OK
from modules.instructions import Instructions
from modules.xmlReader import XMLReader
from sys import argv
from os import path
class Args:
    """
    Classes purpose is to handle the program arguments, check their validity and pass them to further modules
    """
    def __init__(self, stati):
        """
        Initialization function
        :param stati: stati object, for setting output file of stati module
        """
        self.stati = stati
        self.argv = argv.copy()
        self.argv.remove(self.argv[0])
        self.input = ""
        self.source = ""
        self.stats = ""

    def printHelp(self):
        """
        Prints help message to the terminal and successfully ends the program
        """
        print("python3.8 interpret.py <input/source [--input=]/[--source=] [--stats=] [--ints] [--vars]")
        print("Interpreter of IPPcode20")
        print("--help           -Prints this help message, can only by used alone")
        print("--input=file     -Sets file which will be read as input, must be set if source is pipelined to STDIN")
        print("--source=file    -Sets file from which will be read source code, must be set if input is pipelined to "
                                "STDIN")
        print("--stats=file     -Sets file to which will be printed statistics")
        print("--insts           -Number of executed instructions will be printed to stats file, stats must be set")
        print("--vars           -Number of unicialized variables will be printed to stats file, stats must be set")

    def checkHelp(self, argc):
        """
        Checks the presence of --help argument and invalid combinations with help arg
        :param argc: int, count of arguments passed
        :return: Raises Error Args in case of combinations with help, in case of valid use of help arg raises OK
        """
        if argc == 2 and argv[1] == "--help":
            self.printHelp();
            raise OK("Help was printed")
        elif argc > 2 and "--help" in argv:
            raise ErrorArgs("Argument --help can by only used alone with no other arguments, use --help")

    def fileExists(self, filepath):
        """
        Checks wheter path exists and if the path leads to file
        :param filepath: string, path to the file
        :return: true is file exists, false otherwise or if it isnt a file
        """

        if path.exists(path.abspath(filepath)):
            if path.isfile(path.abspath(filepath)):
                return True
            else:
                return False
        else:
            return False

    def findOpts(self, argc):
        """
        Finds the options for viable arguments and removes found viable arguments for ease of further checks
        :param argc: int, count of arguments
        :return: Raises ErrorArgs in case of unexpected token is found or if option is defined more than once
        """
        statsAlready = False
        sourceAlready = False
        inputAlready = False
        for i in range(1, argc):
            if "=" in argv[i]:
                splitArg = argv[i].split("=", 1)
                if splitArg[0] == "--stats":
                    if not statsAlready:
                        if len(splitArg[1]) < 1:
                            raise ErrorArgs("Empty --stats= passed, must be followed by filename")
                        else:
                            self.stats = splitArg[1]
                            self.argv.remove(argv[i])
                            statsAlready = True
                    else:
                        raise ErrorArgs("Option --stats= defined more than once")
                elif splitArg[0] == "--source":
                    if not sourceAlready:
                        self.source = splitArg[1]
                        self.argv.remove(argv[i])
                    else:
                        raise ErrorArgs("Option --source= defined more than once")
                elif splitArg[0] == "--input":
                    if not inputAlready:
                        self.input = splitArg[1]
                        self.argv.remove(argv[i])
                    else:
                        raise ErrorArgs("Option --input= defined more than once")
                else:
                    raise ErrorArgs("Unexpected argument passed to the program")

    def statiArgsCheck(self, stati):
        """
        Checks the arguments of stati module and sets the print order of collected statistics
        :param stati: Stati object
        :return: raises ErrorArgs in case of invalid combination of arguments used or in case of unknown argument
        """
        if self.stats:
            statsOn = True
        else:
            statsOn = False

        for arg in self.argv:
            if arg == "--insts":
                if statsOn:
                    stati.addPrintOrder(stati.INST)
                else:
                    raise ErrorArgs("Instruction --insts was passed to the program but no --stats=file, use --help")
            elif arg == "--vars":
                if statsOn:
                    stati.addPrintOrder(stati.VAR)
                else:
                    raise ErrorArgs("Instruction --vars was passed to the program but no --stats=file, use --help")
            else:
                raise ErrorArgs("Unknown argument passed to the program, use --help")

    def pathsChecks(self, inst, xml):
        """
        Checks the existence of paths to the files and sets the interpreter input and source
        :return: raises ErrorInput in case of non existing or not accessible file path passed to the program
        """
        if not self.input and not self.source:
            raise ErrorInput("No --source nor --input set, at least one must be, read --help")

        if self.input:
            if self.fileExists(self.input):
                inst.setInput(self.input)
            else:
                raise ErrorInput("Invalid path to the input file passed with --input")
        else:
            inst.setInput("STDIN")
            pass

        if self.source:
            if self.fileExists(self.source):
                xml.setSource(self.source)
            else:
                raise ErrorInput("Invalid path to the source file passed with --source")
        else:
            xml.setSource("STDIN")
            pass


    def processArgs(self, inst, xml):
        """
        Processes and checks program arguments
        Sets the parameters of interpreter
        :return: In case of no arguments passed raises ErrorArgs
        """
        # check the number of arguments
        argc = len(argv)
        if argc < 2:
            raise ErrorArgs("No arguments passed to the program, at least one of input/source must be set, use --help")
        # arguments checks
        self.checkHelp(argc)
        self.findOpts(argc)
        self.statiArgsCheck(self.stati)
        # paths checks and setting of paths in other modules
        self.pathsChecks(inst, xml)
        self.stati.setStatiFile(self.stats)





