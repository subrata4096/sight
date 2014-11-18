#include <iostream>
//#include "Python.h"
#include "sight_python.h"
#include <boost/python.hpp>

using namespace boost::python;
static object _sys;
static object _regressModule;
static object _runPyProg;
static std::map<int,object> regressionObjMap;

pythonEnv* pythonEnv::_pyEnv = 0;
pythonEnv::pythonEnv()
{
	std::cout << "creating pythonEnv" << std::endl;


            try {
                    Py_Initialize();
       // Load the sys module.
       _sys = import("sys");
       _sys.attr("path").attr("append")("python_regression");
       //object numpy = import("numpy");
       //object sklearn = import("sklearn");
       _regressModule = import("regressModule");

       _runPyProg = _regressModule.attr("testTry");

     } catch( error_already_set ) {
       PyErr_Print();
    }
 
}

pythonEnv::~pythonEnv()
{
  Py_Finalize();
}
void pythonEnv::init()
{
   _pyEnv = new pythonEnv();
}
void pythonEnv::exit()
{
   if(_pyEnv)
   {
      delete _pyEnv;
      _pyEnv = 0;
   }
}

/*
    o
static const char* MODULE_NAME = "regressModule";

std::string callTestFunc()
{
    PyObject* a = PyImport_Import(PyString_FromString("sys"));
    PyObject* a1 = PyImport_ImportModule("sys");
    PyObject* b = PyImport_Import(PyString_FromString("numpy"));
    PyObject* b1 = PyImport_ImportModule("numpy");
    PyObject* testModule = PyImport_Import(PyString_FromString(MODULE_NAME));
    PyObject* testModule1 = PyImport_ImportModule(MODULE_NAME);
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
*/

 
void pythonEnv::runPython(const std::map<std::string, std::string>& ctxt,
		          const std::map<std::string, std::string>& obs)
{
  if(_pyEnv)
  {
    _runPyProg();
  }
  else
  {
    std::cout<<"python env not initialized" << std::endl;
  }
}



void pythonEnv::loadRegressionObjects(std::string& location, int trace_id)
{
    object obj;	
    regressionObjMap[trace_id] = obj;         
}

