#include <iostream>
#include "Python.h"
#include "sight_python.h"

pythonEnv::pythonEnv()
{
	std::cout << "creating pythonEnv" << std::endl; 
}

static const char* MODULE_NAME = "regressModule";

std::string callTestFunc()
{
    PyObject* testModule = PyImport_Import(PyString_FromString(MODULE_NAME));
    if(!testModule)
    {
	 std::cout << "please make sure you have set proper PYTHONPATH env var" << std::endl;
    }
    PyObject* filterFunc = PyObject_GetAttrString(testModule, "testTry");
     if(!filterFunc)
     {
         std::cout << "please make sure you have set proper PYTHONPATH env var" << std::endl;
     }
    //PyObject* args = Py_BuildValue("(s)", ln.c_str());
    //PyObject* result = PyObject_CallObject(filterFunc, args);
    PyObject* result = PyObject_CallObject(filterFunc, NULL);
    return PyString_AsString(result);
}

void pythonEnv::runPython()
{
	   Py_Initialize();
           callTestFunc();
	   Py_Finalize();
}
