//sgn
#pragma once

/**
 * @file    ExslGPUParser.h
 * @brief   Implements GPU based parsing. NOT FULLY IMPLEMENTED
 * @author  Shankar.KV
 * @date    2025-Mar-21
 * @version 1.0
 * 
 * This file is neither fully implemented nor does it build.
 * This file uses Thrust CPP library to do the operation.
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <memory>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>

#include "ExslParser.h"
#include "ExslGPUParser.h"

class ExslGPUParser : public  {
    public:
        typedef std::shared_ptr<ExslGPUParser> Ptr;
        bool        parseCSVInGPU(int iHandle, const char* ptrBuf);
    
    private:
        ExslGPUParser();
        static ExslGPUParser::Ptr   m_pParser;
        thrust::host_vector<int>    m_hVecOp1, m_hVecOp2, m_hVecRes;
        thrust::device_vector<int>  m_dVecOp1, m_dVecOp2, m_dVecRes;

        const unsigned long MAX_COLs;
        int     m_MapHandle, m_CurOffset;
    
        ExslGPUParser(const ExslGPUParser&)           = delete;
        ExslGPUParser& operator=(const ExslGPUParser&)= delete;
    };