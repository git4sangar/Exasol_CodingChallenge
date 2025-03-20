'''
@file    UnitTests.py
@brief   Contains all Unit Tests
@author  Shankar.KV
@date    2025-Mar-21
@version 1.0

Function names are self explanatory
'''

import datetime
import ExasolParser

def trans_map_empty_param():
    try:
        iHandle = ExasolParser.Exasol_ParseTransformationMap()
    except Exception as e:
        iHandle = -1
    return "PASS" if iHandle == -1 else "FAIL"
    
def trans_map_invalid_param():
    try:
        iHandle = ExasolParser.Exasol_ParseTransformationMap([10, 20, "GfG", 40, True])
    except Exception as e:
        iHandle = -1
    return "PASS" if iHandle == -1 else "FAIL"

def trans_map_invalid_op():
    try:
        trnsMap = {"op1": "", "op3": "sub", "op2": "div", "op4": "mul"}
        iHandle = ExasolParser.Exasol_ParseTransformationMap(trnsMap)
    except Exception as e:
        iHandle = -1
    return "PASS" if iHandle == -1 else "FAIL"

def csv_parse_empty_params():
    try:
        ExasolParser.Exasol_ParseCSV()
    except Exception as e:
        return "PASS"
    return "FAIL"

def csv_parse_invalid_handle():
    try:
        ExasolParser.Exasol_ParseCSV(-1, "op1, 1, 2\nop2, 3, 4")
    except Exception as e:
        return "PASS"
    return "FAIL"

def csv_parse_embedded_null():
    try:
        iHandle = ExasolParser.Exasol_ParseTransformationMap({"op1": "add", "op3": "sub"})
        ExasolParser.Exasol_ParseCSV(iHandle, "op1, 1, 2\nop2, \0, 4")
    except Exception as e:
        return "PASS"
    return "FAIL"

def csv_parse_empty_col():
    try:
        iHandle = ExasolParser.Exasol_ParseTransformationMap({"op1": "add", "op3": "sub"})
        ExasolParser.Exasol_ParseCSV(iHandle, "op1, 1, 2\nop2, , 4")
    except Exception as e:
        return "PASS"
    return "FAIL"

def csv_parse_invalid_val():
    try:
        iHandle = ExasolParser.Exasol_ParseTransformationMap({"op1": "add", "op2": "sub"})
        ExasolParser.Exasol_ParseCSV(iHandle, "op1, 1, 2\nop2, 4a, 4")
    except Exception as e:
        return "PASS"
    return "FAIL"

def csv_parse_missing_cols():
    try:
        iHandle = ExasolParser.Exasol_ParseTransformationMap({"op1": "add", "op2": "sub"})
        ExasolParser.Exasol_ParseCSV(iHandle, "op1, 2\nop2, 3, 4, 5\nop2, 6, 7")
    except Exception as e:
        return "PASS"
    return "FAIL"

def csv_parse_extra_cols():
    try:
        iHandle = ExasolParser.Exasol_ParseTransformationMap({"op1": "add", "op2": "sub"})
        ExasolParser.Exasol_ParseCSV(iHandle, "op1, 1, 2\nop2, 3, 4, 5\nop2, 6, 7")
    except Exception as e:
        return "PASS"
    return "FAIL"

def csv_parse_verify_op_01():
    trnsMap = {"op1": "add", "op2": "sub", "op4": "mul"}
    iHandle = ExasolParser.Exasol_ParseTransformationMap(trnsMap)
    retDict = ExasolParser.Exasol_ParseCSV(iHandle, "op1, 1, 2\nop2, 8, 3\nop3, 6, 7\nop4, 1, 7\nop2, 1, 2")
    actDict = {'op1' : 3, 'op2' : 4, 'op4' : 7}
    return "PASS" if retDict == actDict else "FAIL"

def csv_parse_verify_op_02():
    trnsMap = {"op1": "add", "op2": "div", "op4": "sub"}
    iHandle = ExasolParser.Exasol_ParseTransformationMap(trnsMap)
    retDict = ExasolParser.Exasol_ParseCSV(iHandle, "op1, 1, 2\nop2, 2, 8\nop3, 6, 7\nop4, 1, 7\nop2, 1, 2")
    actDict = {'op1' : 3, 'op2' : 6, 'op4' : -6}
    return "PASS" if retDict == actDict else "FAIL"

def csv_parse_pdf_vals():
    with open("test_files/pdf_inputs.csv", 'r') as file:
        contents    = file.read()
        trnsMap     = {"op1": "add", "op2": "sub", "op4": "mul"}
        iHandle     = ExasolParser.Exasol_ParseTransformationMap(trnsMap)
        retDict     = ExasolParser.Exasol_ParseCSV(iHandle, contents)
        actDict     = {'op1' : 11, 'op2' : -3}
        return "PASS" if retDict == actDict else "FAIL"
    
def csv_parse_huge_file():
    with open("test_files/large_file.csv", 'r') as file:
        contents    = file.read()
        trnsMap     = {"op1": "add", "op2": "div", "op3": "sub", "op4" : "mul"}
        iHandle     = ExasolParser.Exasol_ParseTransformationMap(trnsMap)
        b4          = datetime.datetime.now()
        retVal      = ExasolParser.Exasol_ParseCSV(iHandle, contents)
        after       = datetime.datetime.now()
        deltaTim    = after - b4
        print(f"\n\n--- Time taken for a million rows : {deltaTim.microseconds/1000} milliseconds")
        return retVal

if __name__ == "__main__":
    print("----Validate Transformation Map Inputs------")
    print(f"trans_map_empty_param() {trans_map_empty_param()}")
    print(f"trans_map_invalid_param() {trans_map_invalid_param()}")
    print(f"trans_map_invalid_op() {trans_map_invalid_op()}")
    print("\n\n----Validate CSV Parser Inputs------")
    print(f"csv_parse_empty_params() {csv_parse_empty_params()}")
    print(f"csv_parse_invalid_handle() {csv_parse_invalid_handle()}")
    print(f"csv_parse_embedded_null() {csv_parse_embedded_null()}")
    print(f"csv_parse_empty_col() {csv_parse_empty_col()}")
    print(f"csv_parse_invalid_val() {csv_parse_invalid_val()}")
    print(f"csv_parse_missing_cols() {csv_parse_missing_cols()}")
    print(f"csv_parse_extra_cols() {csv_parse_extra_cols()}")
    print("\n\n----Validate Functionality------")
    print(f"csv_parse_verify_op_01() {csv_parse_verify_op_01()}")
    print(f"csv_parse_verify_op_02() {csv_parse_verify_op_02()}")
    print(f"csv_parse_pdf_vals() {csv_parse_pdf_vals()}")
    print(f"csv_parse_huge_file() {csv_parse_huge_file()}")
