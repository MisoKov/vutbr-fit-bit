from modules.errors import ErrorInternal, ErrorUndefinedFrame, ErrorSemantic, ErrorMissingValue, ErrorUndefinedVariable
from modules.stati import Stati
class Frames:
    """
    Classes purpose is to handle the frames
    """
    def __init__(self, stati):
        """
        Initialization function
        """
        self.frame_stack = []
        self.tempframe = {}
        self.tempdefined = False
        self.global_frame = {}
        self.stati = stati

    def createTemp(self):
        """
        Initializes new temporary frame, old is discarded
        :return: self.tempframe is set to new empty dictionary, self.tempdefined is set to True
        """
        self.tempframe = dict()
        self.tempdefined = True

    def pushTempToStack(self):
        """
        Pushes temporary frame to the top of frame stack
        :return: self.frame_stack has appened tempframe and self.tempdefined is False
        """
        if self.tempdefined == True:
            self.frame_stack.append(self.tempframe)
            self.tempdefined = False
        else:
            raise ErrorUndefinedFrame("Unable to push undefined frame to the top of frame stack")

    def popFromStack(self):
        """
        Pops the frame from top of frame stack to temporary frame
        :return: self.tempframe contains frame from top of frame stack, self.tempdefine is True
        """
        if len(self.frame_stack) >= 1:
            self.tempframe = self.frame_stack.pop()
            self.tempdefined = True
        else:
            raise ErrorUndefinedFrame("Unable to pop from empty stack frame")

    def findFrame(self, frame):
        """
        Finds and return required frame
        :param frame: identificator of frame: GF/LF/TF
        :return: required frame or raises ErrorInternal in case of nad request
        """
        if frame == "GF":
            return self.global_frame
        elif frame == "TF":
            if self.tempdefined == True:
                return self.tempframe
            else:
                raise ErrorUndefinedFrame("Unable to access temporary frame, is undefined")
        elif frame == "LF":
            stack_len = len(self.frame_stack)
            if stack_len >= 1:
                return self.frame_stack[stack_len-1]
            else:
                raise ErrorUndefinedFrame("Local frame undefined, empty stack frame")
        else:
            raise ErrorInternal("Unreachable state, findFrame")

    def splitVarname(self, var):
        """
        Splits full varname to get frame
        :param var: varname of variable also with its location
        :return: tuple of (frame, varname)
        """
        split = var.split("@", 1)
        return split[0], split[1]

    def setVarInFrame(self, var, type, val):
        """
        Sets the value for variable
        :param var: variable, frame and name
        :param type: type of variable
        :param val: value of variable
        :return: raises ErrorUndefinedVariable
        """
        frame_name, varname = self.splitVarname(var)
        frame = self.findFrame(frame_name)
        if varname in frame:
            if frame[varname][0] is None and frame[varname][1] == "":
                #self.stati.incVarCount()
                self.stati.tryAddInst(frame_name, varname)
            frame[varname] = [type, val]
        else:
            raise ErrorUndefinedVariable("Trying to write in to undefined variable")

    def defineVarInFrame(self, var):
        """
        Defines variable in appropriate frame
        :param var: variable, frame and name
        :return: raises ErrorSemantic in case of variable redefinition
        """
        frame_name, varname = self.splitVarname(var)
        frame = self.findFrame(frame_name)
        if varname in frame:
            raise ErrorSemantic("Redefinition of variable: "+str(varname)+" in "+str(frame_name))
        else:
            frame.update({ varname : [None, ""] })

    def getVar(self, var):
        """
        Gets the contents of variable from frame
        :param var: variable, frame and name
        :return: tuple (type, value) or raises ErrorMissingValue or ErrorUndefinedVariable
        """
        frame_name, varname = self.splitVarname(var)
        frame = self.findFrame(frame_name)
        if varname in frame:
            return frame[varname][0], frame[varname][1]
        else:
            raise ErrorUndefinedVariable("Trying to write in to undefined variable")