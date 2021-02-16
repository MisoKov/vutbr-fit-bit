from modules.errors import ErrorOutput
from os.path import abspath

class Stati:
    """
    Class represents the solving of stati module
    """
    # constants
    INST = 0
    VAR = 1

    def __init__(self):
        """
        Inicializator method, inicializes object variables
        """
        self.statiFile = ""
        self.printOrder = []
        self.instCount = 0
        self.initializedVars = {"GF":[],"TF":[],"LF":[]}

    def getInstCount(self):
        """
        Getter functions for self.instCount
        :return: self.instCount
        """
        return self.instCount

    def tryAddInst(self, frame, var):
        """
        Adds variable to list or initialized variables
        """
        if not var in self.initializedVars[frame]:
            self.initializedVars[frame].append(var)

    def countVars(self):
        """
        Counts elements in self.initializedVars
        :return: int count or initialized variables
        """
        return len(self.initializedVars["GF"]) + len(self.initializedVars["LF"]) + len(self.initializedVars["TF"])

    def incInstCount(self):
        """
        Increases object variable instCount by one
        """
        self.instCount += 1

    def addPrintOrder(self, type):
        """
        Appends the type to the end of list printOrder in order to set the printing order of collected statistics
        :param type: Stati class variables INST or VAR
        """
        self.printOrder.append(type)

    def setStatiFile(self, filename):
        """
        Sets the filename for the file to which will be printed the statistics
        :param filename: string, path to file
        """
        self.statiFile = filename

    def printResults(self):
        """
        If printing of results is set, that means statiFile is set and printOrder is not empty, prints the
        corresponding statistic for each element in printOrder on ints own line to the statiFile
        :return: In case of failed attempt at opening the file raises ErrorOutput
        """
        if self.statiFile and len(self.printOrder) > 0:
            try:
                file = open(abspath(self.statiFile), "w")
            except Exception as e:
                raise ErrorOutput(e)
            else:
                for stat in self.printOrder:
                    if stat == Stati.INST:
                        file.write(str(self.instCount)+"\n")
                    elif stat == Stati.VAR:
                        var_count = self.countVars()
                        file.write(str(var_count) + "\n")
                file.close()