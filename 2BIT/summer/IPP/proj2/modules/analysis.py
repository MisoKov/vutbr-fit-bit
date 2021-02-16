from modules.errors import ErrorXMLStruc, ErrorInternal
from modules.xmlReader import XMLReader
class Analysis:
    """
    Classes purpose is to check parsed code, perform the lex, syn a semantic analysis
    """
    def __init__(self):
        """
        Initialization function
        """
        self.code = {}

    def setCode(self, code):
        """
        Setter function for self.code, sets from XMLParser
        :param code: dict containing ordered code
        :return: self.code is set
        """
        self.code = code

    def intCheck(self, integer):
        """
        Checks interger lexical
        :param int: string containing integer
        :return: converted int or raises ErrorXMLstruct in case of invalid int
        """
        try:
            i = int(integer)
        except ValueError:
            raise ErrorXMLStruc("Invalid int: "+str(int))
        else:
            if integer[0] == "-" or integer[0] == "+":
                if not integer[1:].isdigit():
                    raise ErrorXMLStruc("Invalid int: " + str(int))
                else:
                    return i
            if not integer.isdigit():
                raise ErrorXMLStruc("Invalid int: " + str(int))
            else:
                return i

    def stringCheck(self, string):
        """
        Checks string lexically and transforms strings escape sequences
        :param string: string containing string
        :return: valid tranfromed string or raises ErrorXMLStruct in case of invalid string
        """
        if string == None:
            return ""
        string_len = len(string)
        trans_string = ""
        i = 0
        while i < string_len:
            if string[i] == "#":
                raise ErrorXMLStruc("Invalid string: "+str(string))
            elif ord(string[i]) >= 0 and ord(string[i]) <= 32:
                raise ErrorXMLStruc("Invalid string: " + str(string))
            elif string[i] == "\\":
                if i+3 >= string_len:
                    raise ErrorXMLStruc("Invalid string: " + str(string)+" escape seq not finished")
                if not string[i+1].isdigit() or not string[i+2].isdigit() or not string[i+3].isdigit():
                    raise ErrorXMLStruc("Invalid escape sequence in string: " + str(string))
                trans_string += chr(int(string[i + 1] + string[i + 2] + string[i + 3]))
                i += 4
                continue
            trans_string += string[i]
            i += 1
        return trans_string

    def floatCheck(self, f):
        """
        Checks float lexically
        :param f: string containing float
        :return: converted float or raises ErrorXMLStruct in case of invalid float
        """
        try:
            flt = float.fromhex(f)
        except ValueError:
            raise ErrorXMLStruc("Invalid float: "+str(f))
        else:
            return flt

    def nilCheck(self, nil):
        """
        Checks lexicaly value of nil
        :param nil: string containing nil
        :return: raises ErrorXMLStruct in case of invalid nil
        """
        if nil != "nil":
            raise ErrorXMLStruc("Invalid nil value: "+str(nil))

    def boolCheck(self, bool):
        """
        Checks bool lexically
        :param bool: string containing bool
        :return: converted bool or raises ErrorXMLStruct in case of invalid bool
        """
        if bool != "true" and bool != "false":
            raise ErrorXMLStruc("Invalid bool value: "+str(bool))
        elif bool == "true":
            return True
        elif bool == "false":
            return False
        else:
            raise ErrorInternal("Unreachable state boolCheck")

    def checkVarName(self, name):
        """
        Checks lexical validity of name of variable or labelname - same rules apply
        :param name: string containing name of variable of labelname
        :return: raises ErrorXMLStruc in case of invalid name
        """
        name_len = len(name)
        if name_len != 0:
            if not name[0] in ("_", "-", "$", "&", "%", "*", "!", "?") and not name[0].isalpha():
                raise ErrorXMLStruc("Variable name: "+str(name)+" has to star with special char or letter")
            for i in range(1, name_len):
                if name[i].isalnum():
                    continue
                elif name[i] in ("_", "-", "$", "&", "%", "*", "!", "?"):
                    continue
                else:
                    raise ErrorXMLStruc("Invalid variable name: "+str(name))


    def symbolCheck(self, symbol, index_list):
        """
        Determines the type of symbol and calls function to check it
        :param symbol: string containing symbol, can be var or const
        :param index_list: list of ints, indexex leading to symbol in self.code
        :return: raises ErrorXMLStruc in case of invalid symbol
        """
        if symbol[0] == "var":
            self.varCheck(symbol[1])
        elif symbol[0] == "bool":
            b = self.boolCheck(symbol[1])
            self.code[index_list[0]][index_list[1]][1] = b
        elif symbol[0] == "float":
            f =self.floatCheck(symbol[1])
            self.code[index_list[0]][index_list[1]][1] = f
        elif symbol[0] == "string":
            s = self.stringCheck(symbol[1])
            self.code[index_list[0]][index_list[1]][1] = s
        elif symbol[0] == "int":
            i = self.intCheck(symbol[1])
            self.code[index_list[0]][index_list[1]][1] = i
        elif symbol[0] == "nil":
            self.nilCheck(symbol[1])
        else:
            raise ErrorInternal("Unreachable state, symbolCheck")

    def varCheck(self, var):
        """
        Checks variable lexically
        :param var: string containg var also with ist frame
        :return: raises ErrorXMLStruc in case of invalid var
        """
        split_var = var.split("@",1)
        if len(split_var) != 2:
            raise ErrorXMLStruc("Incorrect variable: "+str(var)+" missing frame")
        elif split_var[0] != "GF" and split_var[0] != "LF" and split_var[0] != "TF":
            raise ErrorXMLStruc("Incorrect variable: " + str(var)+" invalid frame")
        else:
            self.checkVarName(split_var[1])

    def labelCheck(self, label):
        """
        Convenience function - checks label lexically, only calls other function
        :param label: string containing labelname
        """
        self.checkVarName(label)

    def readTypeCheck(self, type):
        """
        Checks if type is lexically valid and is one of accepted types
        :param type: string containing one of valid types
        :return: raises ErrorXMLStruc in case of invalid type
        """
        if type != "float" and type != "int" and type != "bool" and type != "string":
            raise ErrorXMLStruc("Invalid type passed for read instruction: "+str(type)+", can be int/float/string/bool")

    def noParamInstCheck(self, inst):
        """
        Checks if instruction which shoul not have any parameters has some parameters
        :param inst: list of instruction and its parameter
        :return: raises ErrorXMLStruc in case of any found parameter
        """
        if inst[1] != None or inst[2] != None or inst[3] != None:
            raise ErrorXMLStruc("Instruction " + str(inst[0]) + " requires 0 arguments")

    def oneParamInstCheck(self, inst, opcode, i):
        """
        Checks if instruction has only one paramater at first position and calls functions to perform further checks on them
        :param inst: list of instruction and its parameter
        :param opcode: string opcode of current instruction
        :param i: index of current instruction in self.code
        :return: raises ErrorXMLStruc in case of missing parameter or additional parameters
        """
        if inst[2] != None or inst[3] != None:
            raise ErrorXMLStruc("Instruction " + str(inst[0]) + " requires only 1 positional argument")
        elif inst[1] == None:
            raise ErrorXMLStruc("Instruction " + str(inst[0]) + " requires 1 argument, 0 given")
        else:
            # symbol
            if opcode in ("PUSHS", "WRITE", "EXIT", "DPRINT"):
                if inst[1][0] in ("var", "string", "float", "int", "bool", "nil"):
                    self.symbolCheck(inst[1], [i, 1])
                else:
                    raise ErrorXMLStruc("Incorrect type of symbol for instruction "+str(inst[0])+": "+str(inst[1][0]))
            # var
            elif opcode in ("DEFVAR", "POPS",):
                if inst[1][0] == "var":
                    self.varCheck(inst[1][1])
                else:
                    raise ErrorXMLStruc("Invalid parameter for instruction " + str(inst[0]) + ": " + str(inst[1][0]))
            # label
            elif opcode in ("CALL", "LABEL", "JUMP", "JUMPIFEQS", "JUMPIFNEQS"):
                if inst[1][0] == "label":
                    self.labelCheck(inst[1][1])
                else:
                    raise ErrorXMLStruc("Invalid parameter for instruction " + str(inst[0]) + ": " + str(inst[1][0]))
            else:
                raise ErrorInternal("Unreachable state reached - oneParamInstCheck")

    def twoParamInstCheck(self, inst, opcode, i):
        """
        Checks if instruction has 2 parameters at first 2 positions and calls functions to perform further checks on them
        :param inst: list of instruction and its parameter
        :param opcode: string opcode of current instruction
        :param i: index of current instruction in self.code
        :return: raises ErrorXMLStruc in case of missing parameters or additional parameter
        """
        if inst[3] != None:
            raise ErrorXMLStruc(
                "Instruction " + str(inst[0]) + " requires only 2 positional arguments, third given: " + str(inst[3]))
        elif inst[1] == None or inst[2] == None:
            raise ErrorXMLStruc("Instruction " + str(inst[0]) + ": missing arguments")
        else:
            # var symbol
            if opcode in ("MOVE", "NOT", "INT2CHAR", "STRLEN", "TYPE", "INT2FLOAT", "FLOAT2INT"):
                if inst[1][0] == "var":
                    self.varCheck(inst[1][1])
                else:
                    raise ErrorXMLStruc("Invalid parameter for instruction " + str(inst[0]) + ": " + str(inst[1][0]))
                if inst[2][0] in ("var", "string", "float", "int", "bool", "nil"):
                    self.symbolCheck(inst[2], [i, 2])
                else:
                    raise ErrorXMLStruc("Incorrect type of symbol for instruction "+str(inst[0])+": "+str(inst[2][0]))
            # var type
            elif opcode == "READ":
                if inst[1][0] == "var":
                    self.varCheck(inst[1][1])
                else:
                    raise ErrorXMLStruc("Invalid parameter for instruction " + str(inst[0]) + ": " + str(inst[1][0]))
                if inst[2][0] == "type":
                    self.readTypeCheck(inst[2][1])
                else:
                    raise ErrorXMLStruc("Invalid parameter for instruction " + str(inst[0]) + ": " + str(inst[2][0]))
            else:
                raise ErrorInternal("Unreachable state reached - twoParamInstCheck")

    def threeParamInstCheck(self, inst, opcode, i):
        """
        Checks if instructions has all 3 parameters and calls functions to perform further checks on them
        :param inst: list of instruction and its parameter
        :param opcode: string opcode of current instruction
        :param i: index of current instruction in self.code
        :return: raises ErrorXMLStruc in case of missing parameters
        """
        if inst[1] == None or inst[2] == None or inst[3] == None:
            raise ErrorXMLStruc("Instruction " + str(inst[0]) + ": missing arguments")
        else:
            # label symb symb
            if opcode in ("JUMPIFEQ", "JUMPIFNEQ"):
                if inst[1][0] == "label":
                    self.labelCheck(inst[1][1])
                else:
                    raise ErrorXMLStruc("Invalid parameter for instruction " + str(inst[0]) + ": " + str(inst[1][0]))
                if inst[2][0] in ("var", "string", "float", "int", "bool", "nil"):
                    self.symbolCheck(inst[2], [i, 2])
                else:
                    raise ErrorXMLStruc("Incorrect type of symbol for instruction "+str(inst[0])+": "+str(inst[2][0]))
                if inst[3][0] in ("var", "string", "float", "int", "bool", "nil"):
                    self.symbolCheck(inst[3], [i, 3])
                else:
                    raise ErrorXMLStruc("Incorrect type of symbol for instruction "+str(inst[0])+": "+str(inst[3][0]))
            # var symb symb
            elif opcode in (
            "CONCAT", "SETCHAR", "GETCHAR", "STRI2INT", "AND", "OR", "LT", "GT", "EQ", "DIV", "IDIV", "MUL", "SUB", "ADD"):
                if inst[1][0] == "var":
                    self.varCheck(inst[1][1])
                else:
                    raise ErrorXMLStruc("Invalid parameter for instruction " + str(inst[0]) + ": " + str(inst[1][0]))
                if inst[2][0] in ("var", "string", "float", "int", "bool", "nil"):
                    self.symbolCheck(inst[2], [i, 2])
                else:
                    raise ErrorXMLStruc(
                        "Incorrect type of symbol for instruction " + str(inst[0]) + ": " + str(inst[2][0]))
                if inst[3][0] in ("var", "string", "float", "int", "bool", "nil"):
                    self.symbolCheck(inst[3], [i, 3])
                else:
                    raise ErrorXMLStruc(
                        "Incorrect type of symbol for instruction " + str(inst[0]) + ": " + str(inst[3][0]))
            else:
                raise ErrorInternal("Unreachable state reached - threeParamInstCheck")

    def runAnalysis(self):
        """
        Runs analysis upon parsed code, calls other functions dependeing on opcode of instruction
        :return: raises ErrorXMLStruc in case of unknown opcode
        """
        for inst in self.code:
            opcode = self.code[inst][0].upper()
            # no parameters instructions
            if opcode in ("CREATEFRAME", "PUSHFRAME", "POPFRAME", "RETURN", "BREAK", "CLEARS", "ADDS", "SUBS",
                          "MULS", "DIVS", "IDIVS", "LTS", "GTS", "EQS", "ANDS", "ORS", "NOTS", "INT2CHARS",
                          "STRI2INTS", "INT2FLOATS", "FLOAT2INTS"):
                self.noParamInstCheck(self.code[inst])
            # 1 param instructions
            elif opcode in ("DEFVAR", "CALL", "PUSHS", "POPS", "WRITE", "LABEL", "JUMP", "EXIT", "DPRINT",
                            "JUMPIFEQS", "JUMPIFNEQS"):
                self.oneParamInstCheck(self.code[inst], opcode, inst)
            # 2 param instructions
            elif opcode in ("MOVE", "INT2CHAR", "NOT", "READ", "STRLEN", "TYPE", "INT2FLOAT", "FLOAT2INT"):
                self.twoParamInstCheck(self.code[inst], opcode, inst)
            # # param instructions
            elif opcode in ("JUMPIFEQ", "JUMPIFNEQ", "SETCHAR", "GETCHAR", "CONCAT", "STRI2INT",
                            "AND", "OR", "LT", "GT", "EQ", "IDIV", "MUL", "SUB", "ADD", "DIV"):
                self.threeParamInstCheck(self.code[inst], opcode, inst)
            else:
                raise ErrorXMLStruc("Invalid opcode: "+str(self.code[inst][0]))