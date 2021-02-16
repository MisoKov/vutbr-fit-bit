from __future__ import print_function
from modules.args import Args
from modules.errors import Error, OK, ErrorInternal
from modules.stati import Stati
from modules.xmlReader import XMLReader
from modules.instructions import Instructions
from modules.analysis import Analysis
from modules.frames import Frames
from sys import stderr

def eprint(*args, **kwargs):
    """
    Convenience function for printing to STDERR
    :param args: Variables ment to be printed
    :param kwargs: Keyworded arguments used in args
    """
    print(*args, file=stderr, **kwargs)

def interpret():
    """
    Main function of program, calls other functions and methods for each subproblem
    Catches the Exceptions and handles them
    If all functions run their course successfully exits program with code OK
    """
    try:
        # initialization of objects
        stati = Stati()
        args = Args(stati)
        frames = Frames(stati)
        inst = Instructions(frames, stati)
        xml = XMLReader()
        analyser = Analysis()
        # preparing interpret, proccessing arguments and reading the source code
        args.processArgs(inst, xml)
        xml.parseXML()
        analyser.setCode(xml.getInstructions())
        analyser.runAnalysis()
        # interpreting - executing of code
        inst.setCode(xml.getInstructions())
        ret = inst.startExecution()
        # printing collected statistics
        stati.printResults()
    except (Error, Exception) as e:
        if issubclass(type(e), Error):
            if isinstance(e, OK):
                exit(OK.code)
            else:
                eprint(e)
                exit(e.code)
        else:
            eprint(e)
            exit(ErrorInternal.code)
    if ret:
        exit(ret)
    else:
        exit(OK.code)

if __name__== "__main__":
    """
    Calls interpret() function
    """
    interpret()
