## Overview
This project is basically a glorified CSV parser with some basic arithmetic operations done on col-1 and col-2 based on the value in the col-0. Finally all the rows have same "op" values are aggregated by summing the values yielded as result of he binary operation.

## Usage
```
# Read contents of a csv file
with open("test_files/pdf_inputs.csv", 'r') as file:
    contents    = file.read()

    # Make a transformation map to parse
    trnsMap     = {"op1": "add", "op2": "sub", "op4": "mul"}
    iHandle     = ExasolParser.Exasol_ParseTransformationMap(trnsMap)

    # Always pass the latest handle returned from Exasol_ParseTransformationMap
    retDict     = ExasolParser.Exasol_ParseCSV(iHandle, contents)
    # Compare the actual against return-Dict
    actDict     = {'op1' : 11, 'op2' : -3}
    return "PASS" if retDict == actDict else "FAIL"

```

## How to setup & build
```
Always have UnitTests.py, setup.py in the same folder as "include and src"
Use the following command to build the CPP files
python3.10 setup.py build_ext --inplace
Now a *.so file will be created and placed in the same directory
You can run the UnitTests.py now by running "python3.10 ./UnitTests.py"

```

