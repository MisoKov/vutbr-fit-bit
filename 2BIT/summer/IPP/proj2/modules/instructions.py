from modules.frames import Frames
import sys
from modules.stati import Stati
from modules.errors import ErrorInternal, ErrorInvalidOperandValue, ErrorMissingValue, ErrorOperandType, ErrorString, \
    ErrorSemantic


class Instructions:
    """
    Class dedicated to executing instructions
    """

    def __init__(self, frames, stati):
        """
        Initialization function
        """
        self.code = {}
        self.labels = {}
        self.frames = frames
        self.stati = stati
        self.callstack = []
        self.datastack = []
        self.program_counter = 0

    def setInput(self, input_source):
        """
        Sets the source of input for the program interpretation
        :param input_source: string, file or STDIN
        """
        if input_source != "STDIN":
            sys.stdin = open(input_source)

    def setCode(self, code):
        """
        Sets the code for interpreting
        :return: self.code is set to dict containing code
        """
        self.code = code

    def isSymbVar(self, symb):
        """
        Checks if symb is defined var, if symb is not var does nothing returns what was passed
        :param symb: symbol
        :return: tuple of (type, value)
        """
        type = symb[0]
        val = symb[1]
        if type == "var":
            type, val = self.frames.getVar(val)
        return type, val

    def popTwo(self):
        """
        Convenience function for stack intructions. Pops 2 values from data stack
        :return: symb2, symb1 from datastack, or raises exception in case of empty data stack
        """
        if len(self.datastack) < 2:
            raise ErrorMissingValue("Error: Trying to access empty value in datastack outside of range")
        else:
            symb2 = self.datastack.pop()
            symb1 = self.datastack.pop()
            return symb2, symb1

    def move(self, var, symb):
        """
        Performs MOVE instruction
        :param var: var location where value will be moved
        :param symb: const value or variable which should be moved
        """
        type, val = self.isSymbVar(symb)
        if type == None:
            raise ErrorMissingValue("Cant MOVE uninitialized variable")
        self.frames.setVarInFrame(var, type, val)

    def createframe(self):
        """
        Performs CREATEFRAME instruction
        :return: new temporary frame is created
        """
        self.frames.createTemp()

    def pushframe(self):
        """
        Performs PUSHFRAME instruction
        :return: temp frame is on top of frame stack
        """
        self.frames.pushTempToStack()

    def popframe(self):
        """
        Performs POPFRAME instruction
        :return: frame from top of frame stack is in temporary frame
        """
        self.frames.popFromStack()

    def defvar(self, var):
        """
        Performs DEFVAR instruction, convenience function only calls other function
        :param var: variable which will be defined
        """
        self.frames.defineVarInFrame(var)

    def call(self, label):
        """
        Performs CALL instruction
        :param label: label where program should jump
        :return: self.program_counter is set to the number corresponding to the order of label
        """
        if label in self.labels:
            self.callstack.append(self.nextInst(self.program_counter))
            self.program_counter = self.labels[label]
        else:
            raise ErrorSemantic("Calling undefined label: " + str(label))

    def return_i(self):
        """
        Performs RETURN instruction
        :return: self.program_counter is set to the value from top of call stack
        """
        if len(self.callstack) < 1:
            raise ErrorMissingValue("No position saved in call stack, can not return to nowhere")
        else:
            self.program_counter = self.callstack.pop()

    def pushs(self, symb):
        """
        Performs PUSHS instruction
        :param symb: variable or const symbol
        :return: symb is on top of data stack
        """
        type, val = self.isSymbVar(symb)
        if type == None:
            raise ErrorMissingValue("Cant PUSHS uninitialized variable")
        self.datastack.append([type, val])

    def pops(self, var):
        """
        Performs POPS instruction
        :param var: variable where value will be stored
        :return: symb from top o data stack is in var
        """
        if len(self.datastack) >= 1:
            symb = self.datastack.pop()
            self.frames.setVarInFrame(var, symb[0], symb[1])
        else:
            raise ErrorMissingValue("Data stack empty, can not perform pops instruction")

    def add(self, var, symb1, symb2):
        """
        Performs ADD instruction
        :param var: variable where result will be stored
        :param symb1: operand of arithmetic operation, const symbol or variable
        :param symb2: operand of arithmetic operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("ADD error: uninitialized variable")
        elif type1 == "int" and type2 == "int":
            res_val = val1 + val2
            res_type = "int"
        elif type1 == "float" and type2 == "float":
            res_val = val1 + val2
            res_type = "float"
        else:
            raise ErrorOperandType("ADD instruction error - bad types of operands, can only be floats or ints")
        self.frames.setVarInFrame(var, res_type, res_val)

    def sub(self, var, symb1, symb2):
        """
        Performs SUB instruction
        :param var: variable where result will be stored
        :param symb1: operand of arithmetic operation, const symbol or variable
        :param symb2: operand of arithmetic operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("SUB error: uninitialized variable")
        elif type1 == "int" and type2 == "int":
            res_val = val1 - val2
            res_type = "int"
        elif type1 == "float" and type2 == "float":
            res_val = val1 - val2
            res_type = "float"
        else:
            raise ErrorOperandType("SUB instruction error - bad types of operands, can only be floats or ints")
        self.frames.setVarInFrame(var, res_type, res_val)

    def mul(self, var, symb1, symb2):
        """
        Performs MUL instruction
        :param var: variable where result will be stored
        :param symb1: operand of arithmetic operation, const symbol or variable
        :param symb2: operand of arithmetic operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("MUL error: uninitialized variable")
        elif type1 == "int" and type2 == "int":
            res_val = val1 * val2
            res_type = "int"
        elif type1 == "float" and type2 == "float":
            res_val = val1 * val2
            res_type = "float"
        else:
            raise ErrorOperandType("MUL instruction error - bad types of operands, can only be floats or ints")
        self.frames.setVarInFrame(var, res_type, res_val)

    def idiv(self, var, symb1, symb2):
        """
        Performs IDIV instruction
        :param var: variable where result will be stored
        :param symb1: operand of arithmetic operation, const symbol or variable
        :param symb2: operand of arithmetic operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or ErrorMissingValue or
                 ErrorInvalidOperandValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("IDIV error: uninitialized variable")
        elif type1 == "int" and type2 == "int":
            if val2 != 0:
                res_val = val1 // val2
                res_type = "int"
            else:
                raise ErrorInvalidOperandValue("IDIV - division by 0")
        else:
            raise ErrorOperandType("IDIV instruction error - bad types of operands, can only be floats or ints")
        self.frames.setVarInFrame(var, res_type, res_val)

    def div(self, var, symb1, symb2):
        """
        Performs DIV instruction
        :param var: variable where result will be stored
        :param symb1: operand of arithmetic operation, const symbol or variable
        :param symb2: operand of arithmetic operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or ErrorMissingValue or
                 ErrorInvalidOperandValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("DIV error: uninitialized variable")
        elif type1 == "float" and type2 == "float":
            if val2 != 0:
                res_val = val1 / val2
                res_type = "float"
            else:
                raise ErrorInvalidOperandValue("DIV - division by 0")
        else:
            raise ErrorOperandType("DIV instruction error - bad types of operands, can only be floats or ints")
        self.frames.setVarInFrame(var, res_type, res_val)

    def lt(self, var, symb1, symb2):
        """
        Performs LT instruction
        :param var: variable where result will be stored
        :param symb1: operand of comparison operation, const symbol or variable
        :param symb2: operand of comparison operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("LT error: uninitialized variable")
        elif type1 == "nil" or type2 == "nil":
            raise ErrorOperandType("Unable to perform less than operation with nil type")
        elif type1 == type2:
            if val1 < val2:
                self.frames.setVarInFrame(var, "bool", True)
            else:
                self.frames.setVarInFrame(var, "bool", False)
        else:
            raise ErrorOperandType("Operands must be of same type when comparing")

    def gt(self, var, symb1, symb2):
        """
        Performs GT instruction
        :param var: variable where result will be stored
        :param symb1: operand of comparison operation, const symbol or variable
        :param symb2: operand of comparison operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("GT error: uninitialized variable")
        elif type1 == "nil" or type2 == "nil":
            raise ErrorOperandType("Unable to perform more than operation with nil type")
        elif type1 == type2:
            if val1 > val2:
                self.frames.setVarInFrame(var, "bool", True)
            else:
                self.frames.setVarInFrame(var, "bool", False)
        else:
            raise ErrorOperandType("Operands must be of same type when comparing")

    def eq(self, var, symb1, symb2):
        """
        Performs EQ instruction
        :param var: variable where result will be stored
        :param symb1: operand of comparison operation, const symbol or variable
        :param symb2: operand of comparison operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("EQ error: uninitialized variable")
        if type1 == "nil" or type2 == "nil":
            if type1 == "nil" and type2 == "nil":
                self.frames.setVarInFrame(var, "bool", True)
            else:
                self.frames.setVarInFrame(var, "bool", False)
            return
        if type1 == type2:
            if val1 == val2:
                self.frames.setVarInFrame(var, "bool", True)
            else:
                self.frames.setVarInFrame(var, "bool", False)
        else:
            raise ErrorOperandType("Operands must be of same type when comparing")

    def and_i(self, var, symb1, symb2):
        """
        Performs AND instruction
        :param var: variable where result will be stored
        :param symb1: operand of logic operation, const symbol or variable
        :param symb2: operand of logic operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("AND error: uninitialized variable")
        elif type1 == "bool" and type2 == "bool":
            res = val1 and val2
            self.frames.setVarInFrame(var, "bool", res)
        else:
            raise ErrorOperandType("Operands must be of type bool when used in logic operation")

    def or_i(self, var, symb1, symb2):
        """
        Performs OR instruction
        :param var: variable where result will be stored
        :param symb1: operand of logic operation, const symbol or variable
        :param symb2: operand of logic operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("OR error: uninitialized variable")
        elif type1 == "bool" and type2 == "bool":
            res = val1 or val2
            self.frames.setVarInFrame(var, "bool", res)
        else:
            raise ErrorOperandType("Operands must be of type bool when used in logic operation")

    def not_i(self, var, symb):
        """
        Performs NOT instruction
        :param var: variable where result will be stored
        :param symb: operand of logic operation, const symbol or variable
        :return: var contains the result or raises ErrorOperandType or
                 ErrorMissingValue in case of error
        """
        type, val = self.isSymbVar(symb)
        if type is None:
            raise ErrorMissingValue("NOT error: uninitialized variable")
        elif type == "bool":
            res = not val
            self.frames.setVarInFrame(var, "bool", res)
        else:
            raise ErrorOperandType("Operand must be of type bool when used not operation")

    def int2char(self, var, symb):
        """
        Performs INT2CHAR instruction
        :param var: variable where result will be stored
        :param symb: const symbol should be int or var containig int
        :return: converted char stored in var or raises ErrorOperandType or ErrorString or
                 ErrorMissingValue in case of error
        """
        type, val = self.isSymbVar(symb)
        if type is None:
            raise ErrorMissingValue("INT2CHAR error: uninitialized variable")
        elif type == "int":
            try:
                res = chr(val)
            except ValueError as e:
                raise ErrorString("INT2CHAR value outside of valid range")
            else:
                self.frames.setVarInFrame(var, "string", res)
        else:
            raise ErrorOperandType("INT2CHAR parameter must be of type int")

    def int2float(self, var, i):
        """
        Performs INT2FLOAT instruction
        :param var: variable where result should be saved
        :param i: integer or name of variable containing integer
        :return: converted float is stored in var  or in case of error raises
                 ErrorMissingValue or ErrorOperandType
        """
        i_type, i_val = self.isSymbVar(i)
        if i_type is None:
            raise ErrorMissingValue("INT2FLOAT error: uninitialized variable")
        elif i_type == "int":
            res = float(i_val)
            self.frames.setVarInFrame(var, "float", res)
        else:
            raise ErrorOperandType("INT2FLOAT parameter must be of type int")

    def float2int(self, var, f):
        """
        Performs FLOAT2INT instruction
        :param var: variable where result should be saved
        :param f: float or name of variable containing float
        :return: converted integer is stored in var or in case of error raises
                 ErrorMissingValue or ErrorOperandType
        """
        f_type, f_val = self.isSymbVar(f)
        if f_type is None:
            raise ErrorMissingValue("FLOAT2INT error: uninitialized variable")
        elif f_type == "float":
            res = int(f_val)
            self.frames.setVarInFrame(var, "int", res)
        else:
            raise ErrorOperandType("FLOAT2INT parameter must be of type float")

    def stri2int(self, var, string, index):
        """
        Performs STRI2INT instruction
        :param var: variable where result will be stored
        :param string: string var of symbol
        :param index: int var or symbol
        :return: converted integer is stored in var or in case of error raises
                 ErrorMissingValue or ErrorOperandType or ErrorString
        """
        s_type, s_val = self.isSymbVar(string)
        i_type, i_val = self.isSymbVar(index)
        if i_type is None or s_type is None:
            raise ErrorMissingValue("STRI2INT error: uninitialized variable")
        elif s_type != "string":
            raise ErrorOperandType("STRIN2INT string parameter must be of type string, is: " + str(s_type))
        elif i_type != "int":
            raise ErrorOperandType("STRI2INT index must be integer, is: " + str(i_type))
        s_len = len(s_val)
        if s_len <= i_val or i_val < 0:
            raise ErrorString("STRI2INT index outside of range")
        else:
            i = ord(s_val[i_val])
            self.frames.setVarInFrame(var, "int", i)

    def read(self, var, type):
        """
        Performs READ instruction
        :param var: variable where read value will be stored
        :param type: type of input we want to read
        :return: read value stored in var or in case of error raises ErrorMissingValue or
                 ErrorInternal
        """
        if type[1] is None:
            raise ErrorMissingValue("READ error: uninitialized variable holding type")
        try:
            read_input = input()
        except EOFError:
            self.frames.setVarInFrame(var, "nil", None)
        else:
            if type[1] == "int":
                try:
                    read_input = int(read_input)
                except ValueError:
                    self.frames.setVarInFrame(var, "nil", None)
                else:
                    self.frames.setVarInFrame(var, "int", read_input)
            elif type[1] == "float":
                try:
                    read_input = float.fromhex(read_input)
                except ValueError:
                    self.frames.setVarInFrame(var, "nil", None)
                else:
                    self.frames.setVarInFrame(var, "float", read_input)
            elif type[1] == "string":
                self.frames.setVarInFrame(var, "string", read_input)
            elif type[1] == "bool":
                if read_input.lower() == "true":
                    self.frames.setVarInFrame(var, "bool", True)
                else:
                    self.frames.setVarInFrame(var, "bool", False)
            else:
                raise ErrorInternal("Unreachable state - read")

    def write(self, symb):
        """
        Performs WRITE instruction
        :param symb: var or const symbol to be written
        :return: raises ErrorMissingValue in case of error
        """
        type, val = self.isSymbVar(symb)
        if type is None:
            raise ErrorMissingValue("WRITE error: uninitialized variable")
        elif type == "nil":
            print("", end='')
        elif type == "bool":
            if val == True:
                print("true", end='')
            else:
                print("false", end='')
        elif type == "float":
            print(val.hex(), end='')
        else:
            print(val, end='')

    def concat(self, var, string1, string2):
        """
        Performs CONCAT instruction
        :param var: variable where result will be stored
        :param string1: string var or const symbol
        :param string2: string var or const symbol
        :return: string result is stored in var or in case of error
                 raises ErrorMissingValue or ErrorOperandType
        """
        type1, val1 = self.isSymbVar(string1)
        type2, val2 = self.isSymbVar(string2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("CONCAT error: uninitialized variable")
        elif type1 == "string" and type2 == "string":
            res = val1 + val2
            self.frames.setVarInFrame(var, "string", res)
        else:
            raise ErrorOperandType("Concat error, can concat only two strings")

    def strlen(self, var, string):
        """
        Performs STRLEN instruction
        :param var: variable where result will be stored
        :param string: string var or const symbol
        :return: int value stored in var or in case of error
                 raises ErrorMissingValue or ErrorOperandType
        """
        type, val = self.isSymbVar(string)
        if type is None:
            raise ErrorMissingValue("STRLEN error: uninitialized variable")
        elif type == "string":
            s_len = len(val)
            self.frames.setVarInFrame(var, "int", s_len)
        else:
            raise ErrorOperandType("STRLEN can be only performed with string")

    def getchar(self, var, string, index):
        """
        Performs GETCHAR instruction
        :param var: variable where result will be stored
        :param string: target string, var or const symbol
        :param index: int var or symbol, position of char
        :return: string value is stored in var or in case of error
                 raises ErrorMissingValue or ErrorOperandType or ErrorString
        """
        s_type, s_val = self.isSymbVar(string)
        i_type, i_val = self.isSymbVar(index)
        if i_type is None or s_type is None:
            raise ErrorMissingValue("GETCHAR error: uninitialized variable")
        elif s_type != "string":
            raise ErrorOperandType("GETCHATS second parameter must be of type string")
        elif i_type != "int":
            raise ErrorOperandType("GETCHATS third parameter must be of type int")
        if i_val >= len(s_val) or i_val < 0:
            raise ErrorString("GETCHAR error, index outside of range")
        self.frames.setVarInFrame(var, "string", s_val[i_val])

    def setchar(self, var, index, char):
        """
        Performs SETCHAR instruction
        :param var: variable where target string is stored
        :param index: int var or symbol, position in var
        :param char: string var or symbol, char
        :return: string changed in var or in case of error
                 raises ErrorMissingValue or ErrorOperandType or ErrorString
        """
        i_type, i_val = self.isSymbVar(index)
        c_type, c_val = self.isSymbVar(char)
        v_type, v_val = self.frames.getVar(var)
        if i_type is None or c_type is None or v_type is None:
            raise ErrorMissingValue("SETCHAR error: uninitialized variable")
        elif v_type != "string":
            raise ErrorOperandType("SETCHAR error variable must be of type string")
        elif c_type != "string":
            raise ErrorOperandType("SETCHAR error symb2-char must be of type string")
        elif i_type != "int":
            raise ErrorOperandType("SETCHAR error symb1-index must be of type int")
        else:
            if i_val >= len(v_val) or i_val < 0:
                raise ErrorString("SETCHAR index out of range")
            elif len(c_val) < 1:
                raise ErrorString("SETCHAR empty symb2-char")
            else:
                res_string = v_val[:i_val] + c_val[0] + v_val[i_val + 1:]
                self.frames.setVarInFrame(var, "string", res_string)

    def type(self, var, symbol):
        """
        Performs TYPE instruction
        :param var: variable where result will be stored
        :param symbol: const symbol or var
        :return: string containing the name of type stored in var
        """
        type, val = self.isSymbVar(symbol)
        if type == None:
            self.frames.setVarInFrame(var, "string", "")
        else:
            self.frames.setVarInFrame(var, "string", type)

    def label(self, label, order):
        """
        Performs LABEL instruction
        :param label: name of label
        :return: label and its order is in labels dict or in case of label redefinition
                 raises ErrorSemantic
        """
        if label in self.labels:
            raise ErrorSemantic("Redefinition of label")
        else:
            self.labels[label] = order

    def jump(self, label):
        """
        Performs JUMP instruction
        :param label: name of label
        :return: self.program_counter is set to the order or corresponding label or in case of
                 undefined label raises ErrorSemantic
        """
        if label in self.labels:
            self.program_counter = self.labels[label]
        else:
            raise ErrorSemantic("Trying to jump to undefined label: " + str(label))

    def jumpifeq(self, label, symb1, symb2):
        """
        Performs JUMPIFEQ instruction
        :param label: name of label
        :param symb1: operand of comparison operation, const symbol or variable
        :param symb2: operand of comparison operation, const symbol or variable
        :return: in case of error raises ErrorMissingValue or ErrorSemantic or ErrorOperandType
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("JUMPIFEQ error: uninitialized variable")
        if not label in self.labels:
            raise ErrorSemantic("JUMPIFEQ undefined label: " + str(label))
        if type1 == "nil" or type2 == "nil":
            if type1 == "nil" and type2 == "nil":
                self.jump(label)
            else:
                self.program_counter = self.nextInst(self.program_counter)
            return
        if type1 == type2:
            if val1 == val2:
                self.jump(label)
            else:
                self.program_counter = self.nextInst(self.program_counter)
        else:
            raise ErrorOperandType("Operands must be of same type when comparing")

    def jumpifneq(self, label, symb1, symb2):
        """
        Performs JUMPIFNEQ instruction
        :param label: name of label
        :param symb1: operand of comparison operation, const symbol or variable
        :param symb2: operand of comparison operation, const symbol or variable
        :return: in case of error raises ErrorMissingValue or ErrorSemantic or ErrorOperandType
        """
        type1, val1 = self.isSymbVar(symb1)
        type2, val2 = self.isSymbVar(symb2)
        if type1 is None or type2 is None:
            raise ErrorMissingValue("JUMPIFNEQ error: uninitialized variable")
        if not label in self.labels:
            raise ErrorSemantic("JUMPIFNEQ undefined label: " + str(label))
        if type1 == "nil" or type2 == "nil":
            if type1 == "nil" and type2 == "nil":
                self.program_counter = self.nextInst(self.program_counter)
            else:
                self.jump(label)
            return
        if type1 == type2:
            if val1 == val2:
                self.program_counter = self.nextInst(self.program_counter)
            else:
                self.jump(label)
            return
        else:
            raise ErrorOperandType("Operands must be of same type when comparing")

    def exit(self, i):
        """
        Performs EXIT instruction
        :param i: int var or symbol
        :return: int return code
        """
        i_type, i_val = self.isSymbVar(i)
        if i_type == None:
            raise ErrorMissingValue("EXIT error: uninitialized variable")
        elif i_type == "int":
            if 0 <= i_val <= 49:
                return i_val
            else:
                raise ErrorInvalidOperandValue("Exit value outside or range")
        else:
            raise ErrorOperandType("Exit code must be int, was given: " + str(i_type))

    def dprint(self, symb):
        """
        Performs DPRINT instruction
        :param symb: var or symbol to be printed on STDERR
        :return: raises ErrorMissingValue in case of error
        """
        type, val = self.isSymbVar(symb)
        if type == None:
            raise ErrorMissingValue("DPRINT error: uninitialized variable")
        elif type == "nil":
            print("", end='', file=sys.stderr)
        elif type == "bool":
            if val == True:
                print("true", end='', file=sys.stderr)
            else:
                print("false", end='', file=sys.stderr)
        else:
            print(val, end='', file=sys.stderr)

    def getOrder(self, inst):
        """
        Returns order of instruction
        :param inst: instruction from self.code
        :return: int key of inst in self.code
        """
        for order, inst_c in self.code.items():
            if inst_c == inst:
                return order

    def break_i(self, inst):
        """
        Performs BREAK instruction
        :param inst: current instruction
        """
        order = self.getOrder(inst)
        print("Order of current instruction: " + str(order), file=sys.stderr)
        print("Number of executed instructions: " + str(self.stati.getInstCount()), file=sys.stderr)
        print("Contents of frames: ", file=sys.stderr)
        print("Global frame: ", file=sys.stderr)
        print(self.frames.global_frame, file=sys.stderr)
        print("Temporary frame: ", file=sys.stderr)
        print(self.frames.tempframe, file=sys.stderr)
        print("Frame stack: ", file=sys.stderr)
        print(self.frames.frame_stack, file=sys.stderr)

    def clears(self):
        """
        Performs CLEARS instruction
        :return: data stack will be empty
        """
        self.datastack.clear()

    def adds(self):
        """
        Performs ADDS instructions
        :return: result of addition will be at the top of the data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == symb2[0]:
            if symb1[0] == "float":
                r_type = "float"
            elif symb1[0] == "int":
                r_type = "int"
            else:
                raise ErrorOperandType("Error ADDS: operands must both be either float or int")
        else:
            raise ErrorOperandType("Error ADDS: operands must be of same type")
        self.datastack.append([r_type, symb1[1] + symb2[1]])

    def subs(self):
        """
        Performs SUBS instructions
        :return: result of substraction will be at the top of the data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == symb2[0]:
            if symb1[0] == "float":
                r_type = "float"
            elif symb1[0] == "int":
                r_type = "int"
            else:
                raise ErrorOperandType("Error SUBS: operands must both be either float or int")
        else:
            raise ErrorOperandType("Error SUBS: operands must be of same type")
        self.datastack.append([r_type, symb1[1] - symb2[1]])

    def divs(self):
        """
        Performs DIVS instructions
        :return: result of division will be at the top of the data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == symb2[0]:
            if symb1[0] == "float":
                if symb2[1] != 0:
                    self.datastack.append(["float", symb1[1] / symb2[1]])
                else:
                    raise ErrorInvalidOperandValue("Error DIVS division by zero")
            else:
                raise ErrorOperandType("Error DIVS: operands must both be float")
        else:
            raise ErrorOperandType("Error DIVS: operands must be of same type")

    def idivs(self):
        """
        Performs IDIVS instructions
        :return: result of division will be at the top of the data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == symb2[0]:
            if symb1[0] == "int":
                if symb2[1] != 0:
                    self.datastack.append(["int", symb1[1] // symb2[1]])
                else:
                    raise ErrorInvalidOperandValue("Error IDIVS division by zero")
            else:
                raise ErrorOperandType("Error IDIVS: operands must both be int")
        else:
            raise ErrorOperandType("Error IDIVS: operands must be of same type")

    def muls(self):
        """
        Performs MULS instructions
        :return: result of multiplication will be at the top of the data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == symb2[0]:
            if symb1[0] == "float":
                r_type = "float"
            elif symb1[0] == "int":
                r_type = "int"
            else:
                raise ErrorOperandType("Error MULS: operands must both be either float or int")
        else:
            raise ErrorOperandType("Error MULS: operands must be of same type")
        self.datastack.append([r_type, symb1[1] * symb2[1]])

    def lts(self):
        """
        Performs LTS instruction
        :return: bool result stored at the top of data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == "nil" or symb2[0] == "nil":
            raise ErrorOperandType("Error LTS: Unable to perform less than operation with nil type")
        elif symb1[0] == symb2[0]:
            if symb1[1] < symb2[1]:
                self.datastack.append(["bool", True])
            else:
                self.datastack.append(["bool", False])
        else:
            raise ErrorOperandType("Error LTS: Operands must be of same type when comparing")

    def gts(self):
        """
        Performs GTS instruction
        :return: bool result stored at the top of data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == "nil" or symb2[0] == "nil":
            raise ErrorOperandType("Error GTS: Unable to perform greater than operation with nil type")
        elif symb1[0] == symb2[0]:
            if symb1[1] > symb2[1]:
                self.datastack.append(["bool", True])
            else:
                self.datastack.append(["bool", False])
        else:
            raise ErrorOperandType("Error GTS: Operands must be of same type when comparing")

    def eqs(self):
        """
        Performs EQS instruction
        :return: bool result stored at the top of data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == "nil" or symb2[0] == "nil":
            if symb1[0] == "nil" and symb2[0] == "nil":
                self.datastack.append(["bool", True])
            else:
                self.datastack.append(["bool", False])
            return
        if symb1[0] == symb2[0]:
            if symb1[1] == symb2[1]:
                self.datastack.append(["bool", True])
            else:
                self.datastack.append(["bool", False])
        else:
            raise ErrorOperandType("Error EQS: Operands must be of same type when comparing")

    def ands(self):
        """
        Performs ANDS instruction
        :return: bool result stored at the top of data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == "bool" and symb2[0] == "bool":
            self.datastack.append(["bool", symb1[1] and symb2[1]])
        else:
            raise ErrorOperandType("Error ANDS: Operands must be of type bool when used in logic operation")

    def ors(self):
        """
        Performs ORS instruction
        :return: bool result stored at the top of data stack or in case of error raises ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if symb1[0] == "bool" and symb2[0] == "bool":
            self.datastack.append(["bool", symb1[1] or symb2[1]])
        else:
            raise ErrorOperandType("Error ORS: Operands must be of type bool when used in logic operation")

    def nots(self):
        """
        Performs NOTS instruction
        :return: bool result stored at the top of data stack or in case of error raises
                ErrorOperandType or ErrorMissingValue
        """
        if len(self.datastack) < 1:
            raise ErrorMissingValue("Error NOTS: Trying to access value in empty datastack")
        symb = self.datastack.pop()
        if symb[0] != "bool":
            raise ErrorOperandType("Error NOTS: Operand must be of type bool in logic operation")
        self.datastack.append(["bool", not symb[1]])

    def int2chars(self):
        """
        Performs INT2CHARS instruction
        :return: string result stored at the top of data stack or in case of error raises
                ErrorOperandType or ErrorMissingValue
        """
        if len(self.datastack) < 1:
            raise ErrorMissingValue("Error INT2CHARS: Trying to access value in empty datastack")
        symb = self.datastack.pop()
        if symb[0] == "int":
            try:
                res = chr(symb[1])
            except ValueError:
                raise ErrorString("Error INT2CHARS: Value outside of valid range")
            else:
                self.datastack.append(["string", res])
        else:
            raise ErrorOperandType("Error INT2CHARS: Parameter must be of type int")

    def stri2ints(self):
        """
        Performs STRI2INTS instruction
        :return: int result stored at the top of data stack or in case of error raises
                ErrorOperandType or ErrorString
        """
        index, string = self.popTwo()
        if string[0] != "string":
            raise ErrorOperandType("Error STRIN2INTS: String parameter must be of type string, is: " + str(string[0]))
        elif index[0] != "int":
            raise ErrorOperandType("Error STRI2INTS: Index must be integer, is: " + str(index[0]))
        s_len = len(string[1])
        if s_len <= index[1] or index[1] < 0:
            raise ErrorString("Error STRI2INTS: Index outside of range")
        else:
            i = ord((string[1])[index[1]])
            self.datastack.append(["int", i])

    def float2ints(self):
        """
        Performs FLOAT2INTS instruction
        :return: int result stored at the top of data stack or in case of error raises
                ErrorOperandType or ErrorMissingValue
        """
        if len(self.datastack) < 1:
            raise ErrorMissingValue("Error FLOAT2INTS: Trying to access value in empty datastack")
        symb = self.datastack.pop()
        if symb[0] == "float":
            res = int(symb[1])
            self.datastack.append(["int", res])
        else:
            raise ErrorOperandType("Error FLOAT2INTS: Parameter must be of type float")

    def int2floats(self):
        """
        Performs INT2FLOATS instruction
        :return: float result stored at the top of data stack or in case of error raises
                ErrorOperandType or ErrorMissingValue
        """
        if len(self.datastack) < 1:
            raise ErrorMissingValue("Error INT2FLOATS: Trying to access value in empty datastack")
        symb = self.datastack.pop()
        if symb[0] == "int":
            res = float(symb[1])
            self.datastack.append(["float", res])
        else:
            raise ErrorOperandType("Error INT2FLOATS: Parameter must be of type int")

    def jumpifeqs(self, label):
        """
        Performs JUMPIFEQS instruction
        :param label: label where program should jump in case of true
        :return: in case of error raises ErrorSemantic or ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if not label in self.labels:
            raise ErrorSemantic("Error JUMPIFEQS: Undefined label: " + str(label))
        if symb1[0] == "nil" or symb2[0] == "nil":
            if symb1[0] == "nil" and symb2[0] == "nil":
                self.jump(label)
            else:
                self.program_counter = self.nextInst(self.program_counter)
            return
        if symb1[0] == symb2[0]:
            if symb1[1] == symb2[1]:
                self.jump(label)
            else:
                self.program_counter = self.nextInst(self.program_counter)
        else:
            raise ErrorOperandType("Error JUMPIFEQS: Operands must be of same type when comparing")

    def jumpifneqs(self, label):
        """
        Performs JUMPIFNEQS instruction
        :param label: label where program should jumpp in case of false
        :return: in case of error raises ErrorSemantic or ErrorOperandType
        """
        symb2, symb1 = self.popTwo()
        if not label in self.labels:
            raise ErrorSemantic("Error JUMPIFNEQS: Undefined label: " + str(label))
        if symb1[0] == "nil" or symb2[0] == "nil":
            if symb1[0] == "nil" and symb2[0] == "nil":
                self.program_counter = self.nextInst(self.program_counter)
            else:
                self.jump(label)
            return
        if symb1[0] == symb2[0]:
            if symb1[1] == symb2[1]:
                self.program_counter = self.nextInst(self.program_counter)
            else:
                self.jump(label)
            return
        else:
            raise ErrorOperandType("Error JUMPIFNEQS: Operands must be of same type when comparing")

    def nextInst(self, current_key):
        """
        Return key of next instruction
        :return: key of next instruction
        """
        next_key = current_key + 1
        keys = list(self.code.keys())
        last_key = keys[-1]
        while not next_key in keys:
            next_key += 1
            if next_key > last_key:
                return False
        return next_key

    def scanForLabels(self, first_inst):
        """
        Scans instructions for labels and sets them in self.labels dictionary
        :param first_inst: order or fisrt instruction
        :return: self.labels is filled with labels and their positions in code
        """
        k = first_inst
        while True:
            if self.code[k][0] == "LABEL":
                self.label(self.code[k][1][1], k)
            k = self.nextInst(k)
            if k == False:
                # end of instructions
                break
        self.program_counter = first_inst

    def startExecution(self):
        """
        Stars the execution of instructions
        :return: int
        """
        #print(self.code)
        if len(self.code) < 1:
            return 0
        self.program_counter = next(iter(self.code))
        self.scanForLabels(self.program_counter)
        while True:
            inst = self.code[self.program_counter]
            #print(inst)
            opcode = inst[0].upper()
            if opcode == "DEFVAR":
                self.defvar(inst[1][1])
            elif opcode == "MOVE":
                self.move(inst[1][1], inst[2])
            elif opcode == "CREATEFRAME":
                self.createframe()
            elif opcode == "POPFRAME":
                self.popframe()
            elif opcode == "PUSHFRAME":
                self.pushframe()
            elif opcode == "CALL":
                self.call(inst[1][1])
                self.stati.incInstCount()
                continue
            elif opcode == "RETURN":
                self.return_i()
                self.stati.incInstCount()
                if self.program_counter == False:
                    # end of instructions
                    break
                continue
            elif opcode == "PUSHS":
                self.pushs(inst[1])
            elif opcode == "POPS":
                self.pops(inst[1][1])
            elif opcode == "ADD":
                self.add(inst[1][1], inst[2], inst[3])
            elif opcode == "SUB":
                self.sub(inst[1][1], inst[2], inst[3])
            elif opcode == "MUL":
                self.mul(inst[1][1], inst[2], inst[3])
            elif opcode == "DIV":
                self.div(inst[1][1], inst[2], inst[3])
            elif opcode == "IDIV":
                self.idiv(inst[1][1], inst[2], inst[3])
            elif opcode == "INT2FLOAT":
                self.int2float(inst[1][1], inst[2])
            elif opcode == "FLOAT2INT":
                self.float2int(inst[1][1], inst[2])
            elif opcode == "GT":
                self.gt(inst[1][1], inst[2], inst[3])
            elif opcode == "LT":
                self.lt(inst[1][1], inst[2], inst[3])
            elif opcode == "EQ":
                self.eq(inst[1][1], inst[2], inst[3])
            elif opcode == "AND":
                self.and_i(inst[1][1], inst[2], inst[3])
            elif opcode == "OR":
                self.or_i(inst[1][1], inst[2], inst[3])
            elif opcode == "NOT":
                self.not_i(inst[1][1], inst[2])
            elif opcode == "INT2CHAR":
                self.int2char(inst[1][1], inst[2])
            elif opcode == "STRI2INT":
                self.stri2int(inst[1][1], inst[2], inst[3])
            elif opcode == "READ":
                self.read(inst[1][1], inst[2])
            elif opcode == "WRITE":
                self.write(inst[1])
            elif opcode == "CONCAT":
                self.concat(inst[1][1], inst[2], inst[3])
            elif opcode == "STRLEN":
                self.strlen(inst[1][1], inst[2])
            elif opcode == "GETCHAR":
                self.getchar(inst[1][1], inst[2], inst[3])
            elif opcode == "SETCHAR":
                self.setchar(inst[1][1], inst[2], inst[3])
            elif opcode == "TYPE":
                self.type(inst[1][1], inst[2])
            elif opcode == "DPRINT":
                self.dprint(inst[1])
            elif opcode == "EXIT":
                self.stati.incInstCount()
                return self.exit(inst[1])
            elif opcode == "LABEL":
                pass
            elif opcode == "JUMP":
                self.jump(inst[1][1])
                self.stati.incInstCount()
                continue
            elif opcode == "JUMPIFEQ":
                self.jumpifeq(inst[1][1], inst[2], inst[3])
                self.stati.incInstCount()
                if self.program_counter == False:
                    # end of instructions
                    break
                continue
            elif opcode == "JUMPIFNEQ":
                self.jumpifneq(inst[1][1], inst[2], inst[3])
                self.stati.incInstCount()
                if self.program_counter == False:
                    # end of instructions
                    break
                continue
            elif opcode == "BREAK":
                self.break_i(inst)
            elif opcode == "CLEARS":
                self.clears()
            elif opcode == "ADDS":
                self.adds()
            elif opcode == "SUBS":
                self.subs()
            elif opcode == "MULS":
                self.muls()
            elif opcode == "IDIVS":
                self.idivs()
            elif opcode == "DIVS":
                self.divs()
            elif opcode == "LTS":
                self.lts()
            elif opcode == "GTS":
                self.gts()
            elif opcode == "EQS":
                self.eqs()
            elif opcode == "ANDS":
                self.ands()
            elif opcode == "ORS":
                self.ors()
            elif opcode == "NOTS":
                self.nots()
            elif opcode == "INT2CHARS":
                self.int2chars()
            elif opcode == "STRI2INTS":
                self.stri2ints()
            elif opcode == "FLOAT2INTS":
                self.float2ints()
            elif opcode == "INT2FLOATS":
                self.int2floats()
            elif opcode == "JUMPIFEQS":
                self.jumpifeqs(inst[1][1])
                self.stati.incInstCount()
                if self.program_counter == False:
                    # end of instructions
                    break
                continue
            elif opcode == "JUMPIFNEQS":
                self.jumpifneqs(inst[1][1])
                self.stati.incInstCount()
                if self.program_counter == False:
                    # end of instructions
                    break
                continue
            else:
                raise ErrorInternal("Unreachable state - startExecution")

            self.stati.incInstCount()
            self.program_counter = self.nextInst(self.program_counter)
            if self.program_counter == False:
                # end of instructions
                break

        return 0
