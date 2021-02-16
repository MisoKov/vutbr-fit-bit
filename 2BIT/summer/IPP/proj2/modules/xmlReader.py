from modules.errors import ErrorXMLFormat, ErrorXMLStruc, ErrorInternal
import xml.etree.ElementTree as ET
from sys import stdin
class XMLReader:
    """
    Class is used for parsing of source xml and checking the correct format
    """
    def __init__(self):
        """
        Initialization function
        """
        self.source = ""
        self.sortedInsts = []
        self.instDict = {}
        self.tree = None

    def setSource(self, source):
        """
        Sets the source of xml code
        :param source: string, file or STDIN
        """
        self.source = source

    def sortInstructions(self):
        """
        Sorts parsed instructions in increasing order
        :return: self.sortedInsts, sorted instructions
        """
        self.sortedInsts = {int(key): val for key, val in sorted(self.instDict.items())}

    def getInstructions(self):
        """
        Getter function for self.sortedInsts
        :return: self.sortedInsts - list of valid sorted instructions and their parameters
        """
        return self.sortedInsts

    def checkRoot(self, root):
        """
        Checks the correct form of root element
        :param root: root element of parsed xml
        :return: raises ErrorXMLStruc in case of invalid root or root attributes
        """
        lang_passed = False
        attrib_len = len(root.attrib)
        if root.tag != "program":
            raise ErrorXMLStruc("Root element has to be 'program', is: "+str(root.tag))
        elif attrib_len > 3 or attrib_len < 1:
            raise ErrorXMLStruc("Too many or no attributes set for root element program, language attribute is required")


        keys = root.attrib.keys()
        for k in keys:
            if k == "description":
                continue
            elif k == "name":
                continue
            elif k == "language":
                if root.attrib["language"] != "IPPcode20":
                    raise ErrorXMLStruc("Language of program unrecognized, has to be 'IPPcode20'")
                else:
                    lang_passed = True
            else:
                raise ErrorXMLStruc("Unknown program attribute")

        if not lang_passed:
            raise ErrorXMLStruc("Program language not specified, has to be 'IPPcode20'")

    def orderCheck(self, order):
        """
        Runs all checks on order
        :param order: string representation of order
        :return: raises ErrorXMLStruc in case of invalid order
        """
        # positive integer
        if not order.isdigit():
            raise ErrorXMLStruc("Invalid order value, should be positive integer larger than 0")
        # zero
        if order == "0":
            raise ErrorXMLStruc("Invalid order value, is 0, should be larger than 0")
        # checking duplicity of order
        if int(order) in self.instDict:
            raise ErrorXMLStruc("Order duplicity")


    def runAllChecks(self):
        """
        Convenience function, runs all checks with parsed xml, raises ErrorXMLStruc in case of failed checks
        :return: self.instDic, dictionary of instruction and their arguments
        """
        # root check
        if isinstance(self.tree, ET.ElementTree):
            root = self.tree.getroot()
        elif isinstance(self.tree, ET.Element):
            root = self.tree
        else:
            raise ErrorInternal("xmlReader - runAllChecks - root check unreachable state")
        self.checkRoot(root)
        # instructions checks
        for inst in root:
            inst_el = [None]*4
            if inst.tag != "instruction":
                raise ErrorXMLStruc("Unexpected element in program structure, should be only 'instruction'")
            elif len(inst.attrib) != 2:
                raise ErrorXMLStruc("Incorrect number of instruction attributes, should be 2 - order, opcode")
            if not ("order" in inst.attrib and "opcode" in inst.attrib):
                raise ErrorXMLStruc("Unknown attribute of instruction found, should be order, opcode")
            inst_el[0] = inst.attrib["opcode"]

            # inst arguments check
            arg1_set = False
            arg2_set = False
            arg3_set = False
            for arg in inst:
                if len(arg) != 0:
                    raise ErrorXMLStruc("Arguments should not have any children elements")
                elif len(arg.attrib) != 1:
                    raise ErrorXMLStruc("Argument has incorrect count of attributes")
                elif not "type" in arg.attrib:
                    raise ErrorXMLStruc("Argument has unknown attribute specified, should be only type")
                elif arg.tag == "arg1":
                    if arg1_set:
                        raise ErrorXMLStruc("Multiple arguments passed as arg1")
                    arg1_set = True
                    inst_el[1] = [arg.attrib["type"], arg.text]
                elif arg.tag == "arg2":
                    if arg2_set:
                        raise ErrorXMLStruc("Multiple arguments passed as arg2")
                    arg2_set = True
                    inst_el[2] = [arg.attrib["type"], arg.text]
                elif arg.tag == "arg3":
                    if arg3_set:
                        raise ErrorXMLStruc("Multiple arguments passed as arg3")
                    arg3_set = True
                    inst_el[3] = [arg.attrib["type"], arg.text]
                else:
                    raise ErrorXMLStruc("Unknown element found as child of instruction")
            # order check
            self.orderCheck(inst.attrib["order"])
            # adding inst_el to the dictionary
            self.instDict.update( {int(inst.attrib["order"]) : inst_el} )

    def parseXML(self):
        """
        Parses the code from source
        :return: self.tree, xml tree containing source code
        """
        # parsing of xml
        if self.source == "STDIN":
            source_str = stdin.read()
            try:
                self.tree = ET.fromstring(source_str, ET.XMLParser(encoding='utf-8'))
            except Exception:
                raise ErrorXMLFormat("Not well formed")
        else:
            try:
                self.tree = ET.parse(self.source, ET.XMLParser(encoding='utf-8'))
            except Exception:
                raise ErrorXMLFormat("Not well formed")
        # checking correct structure
        self.runAllChecks()
        # sorting checked xml into list
        self.sortInstructions()