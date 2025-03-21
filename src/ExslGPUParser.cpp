//sgn
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "ExslGPUParser.h"

/**
 * @file    ExslGPUParser.cpp
 * @brief   Implements GPU based parsing. NOT FULLY IMPLEMENTED
 * @author  Shankar.KV
 * @date    2025-Mar-21
 * @version 1.0
 * 
 * This file is neither fully implemented nor does it build.
 * This file uses Thrust CPP library to do the operation.
 */

struct add_functor {
    __host__ __device__
    int operator()(const int& x, const int& y) const {
        return x + y;
    }
};

ExslGPUParser::Ptr ExslGPUParser::m_pParser = nullptr;

ExslGPUParser::Ptr ExslGPUParser::getInstance() {
    if(!m_pParser) m_pParser = Ptr(new ExslGPUParser());
    return m_pParser;
}

ExslGPUParser::ExslGPUParser() {}


bool ExslGPUParser::parseCSVInGPU(int iHandle, const char* ptrBuf) {
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

        //  Assuing op1 is Add
        if(aRow[0] == "op1") {
            m_hVecOp1.push_back(std::stoi(aRow[1]));
            m_hVecOp2.push_back(std::stoi(aRow[2]));
        }
    }
    m_dVecOp1 = m_hVecOp1;
    m_dVecOp2 = m_hVecOp2;
    thrust::transform(m_dVecOp1.begin(), m_dVecOp1.end(), m_dVecOp2.begin(), m_dVecRes.begin(), add_functor());
    double finalRes = thrust::reduce(m_dVecRes.begin(), m_dVecRes.end(), 0, thrust::plus<int>());
    
    return true;
}