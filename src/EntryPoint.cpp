/**
 * @file    EntryPoint.cpp
 * @brief   Acts as an interface btwn Python & Cpp
 * @author  Shankar.KV
 * @date    2025-Mar-21
 * @version 1.0
 * 
 * Function names are self explanatory
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <string>
#include <algorithm>

#include "ExslParser.h"
#include "ExslGPUParser.h"

static PyObject* Exasol_ParseCSV(PyObject* self, PyObject* args) {
    const char* pBuffer;
    int iHandle = 0;
    if(!PyArg_ParseTuple(args, "is", &iHandle, &pBuffer)) {
        return NULL;
    }
    auto pParser = ExslParser::getInstance();
    if(!pParser->isHandleOk(iHandle)) {
        PyErr_SetString(PyExc_ValueError, "Invalid Handle");
        return NULL;
    }

    if(!pParser->parseCSVInCPU(iHandle, pBuffer)) {
        PyErr_SetString(PyExc_ValueError, "Invalid CSV content");
        return NULL;
    }

    PyObject* pPyDict   = pParser->packAggregate();
    if(!pPyDict) {
        PyErr_SetString(PyExc_ValueError, "Error converting result to PyDict");
        return NULL;
    }
    return pPyDict;
}

//  Returns a handle to be passed to Exasol_ParseCSV
//  This way it is ensured that this function has to invoked before Exasol_ParseCSV
static PyObject* Exasol_ParseTransformationMap(PyObject* self, PyObject* args) {
    PyObject* pDictObj;

    if(!PyArg_ParseTuple(args, "O", &pDictObj)) {
        return NULL;
    }
    auto pParser    = ExslParser::getInstance();
    int iHandle     = pParser->parseTransformationMap(pDictObj);
    if(0 > iHandle) {
        PyErr_SetString(PyExc_ValueError, "Invalid input or Error converting datatype");
        return NULL;
    }
    return PyLong_FromLong(iHandle);
}

static PyObject* version(PyObject* self) {
    return Py_BuildValue("s", "1.0.0");
}

static PyMethodDef Exasol_APIs[] = {
    {   "Exasol_ParseCSV",
        Exasol_ParseCSV,
        METH_VARARGS,
        "Processes the CSV in CPU."
    },
    {   "Exasol_ParseTransformationMap",
        Exasol_ParseTransformationMap,
        METH_VARARGS,
        "Gets the Transformation Map"
    },
    {"version", (PyCFunction)version, METH_NOARGS, "returns version info"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef ExasolParserMod = {
    PyModuleDef_HEAD_INIT,
    "ExasolParser",
    "ParseCSV",
    -1,
    Exasol_APIs
};

PyMODINIT_FUNC PyInit_ExasolParser(void) {
    return PyModule_Create(&ExasolParserMod);
}