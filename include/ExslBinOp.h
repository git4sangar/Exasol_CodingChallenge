#pragma once

/**
 * @file    ExslBinOp.h
 * @brief   Implements all the binay operations required for the parser.
 * @author  Shankar.KV
 * @date    2025-Mar-21
 * @version 1.0
 * 
 * ExslBinOp is the base class from which each operations get derived 
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <memory>

//  From this, all other operations are derived
class ExslBinOp {
public:
    typedef std::shared_ptr<ExslBinOp> Ptr;
    std::string m_OpName;

    ExslBinOp(const std::string& opName) : m_OpName {opName}{}
    virtual ~ExslBinOp() {}

    virtual std::string getOpName() = 0;

    //  "operate" performs the actual operation based on the derived class
    virtual double operate(const std::string&, const std::string&) = 0;
};

//  Adds pStrOp1 and pStrOp2
class OpAdd : public ExslBinOp {
public:
    typedef std::shared_ptr<OpAdd> Ptr;
    OpAdd() : ExslBinOp("ADD") {}
    virtual ~OpAdd() {}

    std::string getOpName() { return m_OpName; }

    double operate(const std::string& pStrOp1, const std::string& pStrOp2) {
        double iSum = 0.0;
        iSum = std::stoi(pStrOp1) + std::stoi(pStrOp2);
        return iSum;
    }
};

//  Subtracts the value of pStrOp2 from pStrOp1
class OpSub : public ExslBinOp {
public:
    typedef std::shared_ptr<OpSub> Ptr;
    OpSub() : ExslBinOp("SUB") {}
    virtual ~OpSub() {}

    std::string getOpName() { return m_OpName; }

    double operate(const std::string& pStrOp1, const std::string& pStrOp2) {
        double iDiff = 0.0;
        iDiff = std::stoi(pStrOp1) - std::stoi(pStrOp2);
        return iDiff;
    }
};

//  Multiplies the value of pStrOp2 and pStrOp1
class OpMul : public ExslBinOp {
public:
    typedef std::shared_ptr<OpMul> Ptr;
    OpMul() : ExslBinOp("MUL") {}
    virtual ~OpMul() {}

    std::string getOpName() { return m_OpName; }

    double operate(const std::string& pStrOp1, const std::string& pStrOp2) {
        double iProd = 0.0;
        iProd = std::stoi(pStrOp1) * std::stoi(pStrOp2);
        return iProd;
    }
};

//  Divide the value of pStrOp2 by pStrOp1
class OpDiv : public ExslBinOp {
public:
    typedef std::shared_ptr<OpMul> Ptr;
    OpDiv() : ExslBinOp("DIV") {}
    virtual ~OpDiv() {}

    std::string getOpName() { return m_OpName; }

    double operate(const std::string& pStrOp1, const std::string& pStrOp2) {
        double dQtn = 0.0;
        dQtn = std::stod(pStrOp2) / std::stod(pStrOp1);
        return dQtn;
    }
};

class OpPer : public ExslBinOp {
public:
    typedef std::shared_ptr<OpMul> Ptr;
    OpPer() : ExslBinOp("PER") {}
    virtual ~OpPer() {}

    std::string getOpName() { return m_OpName; }

    double operate(const std::string& pStrOp1, const std::string& pStrOp2) {
        double dPer = 0.0;
        dPer = std::stod(pStrOp2)  * 100 / std::stod(pStrOp1);
        return dPer;
    }
};