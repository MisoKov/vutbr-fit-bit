class Error(Exception):
    """
    Base class for errors from which specific errors inherit
    """
    def __init__(self, message):
        """
        Inicialization function of all errors
        :param message: Error message
        """
        self.message = message

    def getMsg(self):
        """
        Returns error message
        :return: Message of the error
        """
        return self.message

class OK(Error):
    """
    Program run successuly, used only to end program early
    """
    code = 0

class ErrorArgs(Error):
    """
    Error occurs when incorrect combination of arguments are passed to the program
    or when unknown arguments are passed or when required arguments are not passed
    or when no arguments are passed
    """
    code = 10

class ErrorInput(Error):
    """
    Error occurs when the unexisting input/path the the input files are passed to the program, or files cant be opened
    """
    code = 11

class ErrorOutput(Error):
    """
    Error occurs when output file couldt be openned, does not exist, permission obstacle...
    """
    code = 12

class ErrorXMLFormat(Error):
    """
    Errors occurs in case of invalid XML format of source code - XML Parsing Error: not well-formed
    """
    code = 31

class ErrorXMLStruc(Error):
    """
    Error occurs in case of unexpected structure was found in source code
    """
    code = 32

class ErrorSemantic(Error):
    """
    Errors occurs when existing label or variable is redefined
    """
    code = 52

class ErrorOperandType(Error):
    """
    Error occurs when invalid combination of operand types are used in comparison
    """
    code = 53

class ErrorUndefinedVariable(Error):
    """
    Error occurs when trying to access undefined variable (frame exists)
    """
    code = 54

class ErrorUndefinedFrame(Error):
    """
    Error occurs when undefined frame is gonna be accessed
    """
    code = 55


class ErrorMissingValue(Error):
    """
    Value not found, in variable or in frame stack or call stack
    """
    code = 56


class ErrorInvalidOperandValue(Error):
    """
    Division by zero or invalid return value of EXIT instruction
    """
    code = 57


class ErrorString(Error):
    """
    Invalid string operation
    """
    code = 58

class ErrorInternal(Error):
    """
    Error occurs in case of internal program failure
    """
    code = 99