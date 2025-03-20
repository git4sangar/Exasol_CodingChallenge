#pragma once
/**
 * @file    ExslParser.h
 * @brief   Implements the core parsing logic
 * @author  Shankar.KV
 * @date    2025-Mar-21
 * @version 1.0
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ExslBinOp.h"

//  Singleton class
class ExslParser {
public:
    typedef std::shared_ptr<ExslParser> Ptr;

    static ExslParser::Ptr      getInstance();
    ExslBinOp::Ptr              getOpObj(const std::string &);
    std::vector<std::string>    parseARow(const char* ptrBuf);
    virtual ~ExslParser()       { m_OpPtrs.clear(); m_TrnsMap.clear(); }

    bool        parseCSVInCPU(int iHandle, const char* ptrBuf);
    int         getCurOffset()          { return m_CurOffset; }
    int         getCurHandle()          { return m_MapHandle; }
    bool        isHandleOk(int iHandle) { return (iHandle == m_MapHandle); }
    int         parseTransformationMap(PyObject *pPyObj);
    PyObject*   packAggregate();

private:
    ExslParser();
    static ExslParser::Ptr                          m_pParser;
    std::unordered_map<std::string, ExslBinOp::Ptr> m_OpPtrs;
    std::unordered_map<std::string, std::string>    m_TrnsMap;
    std::unordered_map<std::string, double>         m_Aggregate;

    const unsigned long MAX_COLs;
    int     m_MapHandle, m_CurOffset;
    bool    isDigit(const std::string& pStr);

    ExslParser(const ExslParser&)           = delete;
    ExslParser& operator=(const ExslParser&)= delete;
};
