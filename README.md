## Overview
This project is an advanced CSV parser designed to execute binary operations based on user-defined mappings. It processes a CSV file with the following structure:

Column 1 (Mnemonic): Contains mnemonics representing binary operations.
Column 2 (Operand 1): Contains the first numerical operand.
Column 3 (Operand 2): Contains the second numerical operand.
The mnemonics are mapped to their corresponding binary operations, such as addition, subtraction, multiplication, or division, using a Python dictionary provided by the user. The parser performs the specified operation on the values in columns 2 and 3. The results of each operation type are aggregated and presented as a Python dictionary, providing a concise summary of the computations.

## Usage
```
#import the Exasol module
import ExasolParser

# Read contents of a csv file
with open("filename.csv", 'r') as file:
    contents    = file.read()

    # Use a python dictionary to map an operation to each mnemonic
    trnsMap     = {"op1": "add", "op2": "sub", "op4": "mul"}

    # This parses the transformation map and returns a handle
    # Param:    A dict that maps a mnemonic to a 2 operand-operation
    # Return:   A handle to be passed as a param to other APIs
    # Note:     Handle returned by most recent invocation alone is valid
    # As other APIs require this handle, the order of invocation of APIs is ensured
    iHandle     = ExasolParser.Exasol_ParseTransformationMap(trnsMap)

    # Parses a csv buffer as said in Overview and returns a dict
    # Param:    Handle returned from last invocation of Exasol_ParseTransformationMap
    # Return:   A dictionary with
    #   Keys :  mnemonics as keys
    #   Values: Sum of all results of ops mapped by this mnemonic
    retDict     = ExasolParser.Exasol_ParseCSV(iHandle, contents)
```

## How to build & run
```
Clone the repository into a desired folder
user@host:~/projs$ git clone git@github.com:git4sangar/Exasol_CodingChallenge.git
user@host:~/projs$ cd Exasol_CodingChallenge

Build the cpp sources as follows. A .so file will be created
user@host:~/projs/Exasol_CodingChallenge$ python3.10 setup.py build_ext --inplace

Now run the Example.py as follows
user@host:~/projs/Exasol_CodingChallenge$ python3.10 ./Example.py

Unit Tests are executed as follows
user@host:~/projs/Exasol_CodingChallenge$ python3.10 ./UnitTests.py

```

## Implementation
Major portion of the code is written in C++. Containers from STL are used as appropriate. Other than that care is taken not to use third party libraries. Even the parsing of CSV is handcoded from the scratch. To avoid such dependencies with 3rd party libs, python itself is used for writing the Unit Test cases.
The ExslParser class is the place where major chunk of parsing logic is implemented. The idea here is to parse the CSV content row by row. While parsing each row the said operation is performed and results are accumulated in a map as values as against their appropriate mnemonic as key for the map. All the operations are implemented as virtual functions. By this one can achieve scalability of including more operations in the future. For eg: operations like Percentage is included.
### Code Readability
Care is taken to make sure each function has less than 100 lines of code. Also each function is well defined to do one operation which the name of it reflects exactly.
### CPU & Memory Utilization
For performance reasons, unordered map is used wherever required. As umap implements hash-functions, the searches are done in O(1).
To coserve memory, the same input buffer from python is utilized as it is. Meaning, the parsing and operations are done each row-wise in single traverse in the original buffer without copying. Such an approach is mandatory for huge files with millions of rows.
### GPU Usage
Disclaimer: The GPU based parsing is neither fully implemented nor built. It is just PoC code base. If at all one needs to implement, he can follow the approach used ExslGPUParser.h and ExslGPUParser.cpp. 
There is a PoC level implementation for performing such parsing using GPU too. But as it depends on Nividia drivers, it is not built as part of the build-script.
