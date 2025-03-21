/**
 * @file    ExslParser.h
 * @brief   Implements the core parsing logic
 * @author  Shankar.KV
 * @date    2025-Mar-21
 * @version 1.0
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

#include "ExslParser.h"
#include "ExslBinOp.h"

ExslParser::Ptr ExslParser::m_pParser = nullptr;

ExslParser::Ptr ExslParser::getInstance() {
    if(!m_pParser) m_pParser = Ptr(new ExslParser());
    return m_pParser;
}

ExslParser::ExslParser() :
    MAX_COLs(3),
    m_MapHandle(0),
    m_CurOffset(0) {
    m_OpPtrs.insert({ "ADD", std::make_shared<OpAdd>() });
    m_OpPtrs.insert({ "SUB", std::make_shared<OpSub>() });
    m_OpPtrs.insert({ "MUL", std::make_shared<OpMul>() });
    m_OpPtrs.insert({ "DIV", std::make_shared<OpDiv>() });
    m_OpPtrs.insert({ "PER", std::make_shared<OpPer>() });
}

//  Returns the binary-operator-object matching the "pStrOp"
//  if pStrOp is "mul", this function returns ptr to Mul Obj
ExslBinOp::Ptr ExslParser::getOpObj(const std::string &pStrOp) {
    ExslBinOp::Ptr  pBinOp;
    std::string     strVal;

    //  Get the operator name
    if(m_TrnsMap.find(pStrOp) != m_TrnsMap.end())
        strVal = m_TrnsMap.at(pStrOp);

    //  Get the object corresponding to operator
    if(m_OpPtrs.find(strVal) != m_OpPtrs.end())
        pBinOp = m_OpPtrs.at(strVal);
    return pBinOp;
}

//  Parses the user-provided Transformation Map. Eg "op1" : "add"
//  Holds the key / value in m_TrnsMap which is an unordered-map
//  m_TrnsMap will contain {"op1", "ADD"} & so on
int ExslParser::parseTransformationMap(PyObject *pPyObj) {
    if(!pPyObj || !PyDict_Check(pPyObj)) return -1;

    PyObject *pPyKeys   = PyDict_Keys(pPyObj);
    PyObject *pPyVals   = PyDict_Values(pPyObj);
    Py_ssize_t pyiSz    = PyDict_Size(pPyObj);
    if(!pPyKeys || !pPyVals || !pyiSz) return -1;

    //  Parse each key, val in dict; validate & populate m_TrnsMap
    std::string strValUp;
    int iLoop = 0;
    m_TrnsMap.clear();
    for(iLoop = 0; iLoop < pyiSz; iLoop++) {
        PyObject *pKey  = PyList_GetItem(pPyKeys, iLoop);
        PyObject *pVal  = PyList_GetItem(pPyVals, iLoop);
        if(pKey && PyUnicode_Check(pKey) && pVal && PyUnicode_Check(pVal)) {
            const char* strKey  = PyUnicode_AsUTF8(pKey);
            const char* strVal  = PyUnicode_AsUTF8(pVal);
            strValUp            = std::string(strVal);
            std::transform(strValUp.begin(), strValUp.end(), strValUp.begin(),
                [](char c){ return std::toupper(c);}
            );
            if(m_OpPtrs.find(strValUp) == m_OpPtrs.end()) break;
            m_TrnsMap.insert({ std::string(strKey), std::string(strValUp) });
        }
        else break;
    }

    Py_DECREF(pPyVals);
    Py_DECREF(pPyKeys);
    return (iLoop == pyiSz) ? ++m_MapHandle : -1;
}

//  Parses the buffer row by row. During each row, perform the operation
//  The results are accumulated in m_Aggregate against the mnemonic
bool ExslParser::parseCSVInCPU(int iHandle, const char* ptrBuf) {
    if(iHandle != m_MapHandle) return false;

    m_CurOffset = 0;
    int iBufLen = std::strlen(ptrBuf);
    double dVal = 0.0;
    m_Aggregate.clear();

    //  Loop through entire buffer
    while(m_CurOffset < iBufLen) {
        auto aRow = parseARow(ptrBuf + m_CurOffset);
        if(MAX_COLs != aRow.size())         return false;
        for(auto col : aRow) if(col.empty())return false;

        const auto& strOp       = aRow[0];
        ExslBinOp::Ptr pBinOp   = getOpObj(strOp);
        if(pBinOp) {
            //  Check and perform operation
            if(!isDigit(aRow[1]) || !isDigit(aRow[2])) return false;
            try { dVal = pBinOp->operate(aRow[1], aRow[2]); }
            catch(...) { return false; }

            //  Aggregate each result and place it against the col-1
            if(m_Aggregate.find(strOp) == m_Aggregate.end())
                m_Aggregate.insert({ strOp, 0 });
            m_Aggregate[strOp] += dVal;
        }
    }
    return true;
}

//  Parses a single row in a lengthy buffer
std::vector<std::string> ExslParser::parseARow(const char* ptrBuf) {
    std::vector<std::string> aRow;
    std::string aCol;
    if(!ptrBuf) return aRow;

    for(; *ptrBuf && '\n' != *ptrBuf; ptrBuf++, m_CurOffset++) {
        if(aCol.empty() && std::isspace(*ptrBuf)) continue;

        if(',' == *ptrBuf || '\n' == *ptrBuf) {
            aRow.push_back(aCol);
            aCol.clear();
        } else aCol += *ptrBuf;
    }
    m_CurOffset++;
    if(!aCol.empty()) aRow.push_back(aCol);
    return aRow;
}

//  Pack a dictionary to send to python
//  Sum of results of each menmonics (like op1, op2) are all available in m_Aggregate
//  Convert the same as a python dictionary
PyObject* ExslParser::packAggregate() {
    PyObject* pPyDict = PyDict_New();
    if(!pPyDict) return NULL;

    for(const auto& pair : m_Aggregate) {
        PyObject* pPyKey    = PyUnicode_FromString(pair.first.c_str());
        PyObject* pPyVal    = PyFloat_FromDouble(pair.second);
        if(!pPyKey || !pPyVal) return NULL;
        PyDict_SetItem(pPyDict, pPyKey, pPyVal);

        Py_DECREF(pPyKey);
        Py_DECREF(pPyVal);
    }
    return pPyDict;
}

bool ExslParser::isDigit(const std::string& pStr) {
    return std::all_of(pStr.begin(), pStr.end(), ::isdigit);
}