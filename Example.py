import ExasolParser

# Use a python dictionary to map an operation to each mnemonic
trnsMap     = {"op1": "add", "op2": "sub", "op3": "mul", "op4" : "div"}

# Following function parses the transformation map and returns a handle
# Param:    A dictionary that maps a mnemonic to an operation that takes 2 operands
# Return:   A handle that has to be passed as a parameter to APIs like Exasol_ParseCSV
# Note:     The handle returned by the most recent invocation of this call alone is valid
# As other APIs require this handle to be passed as param, the order of invocation of APIs is ensured
iHandle     = ExasolParser.Exasol_ParseTransformationMap(trnsMap)

# Parses a csv buffer as stated in Overview and returns a dictionary containing the results
# Param:    The handle returned from most recent invocation of Exasol_ParseTransformationMap
# Return:   A dictionary with
#   Keys :  mnemonics as keys
#   Values: Sum of all the results of operations mapped by this mnemonic
retDict     = ExasolParser.Exasol_ParseCSV(iHandle, "op1, 1, 2\nop2, 4, 3\nop3, 5, 6\nop4, 1, 2")

print(retDict)