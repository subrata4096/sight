#include <iostream>
#include <string>
//#include "Python.h"
#include "sight_python.h"
#include <boost/python.hpp>

using namespace boost::python;
static object _sys;
static object _regressModule;
static object _runPyProg;
static object _runPypredictValue;
static object _runPyLoadModel;
//static std::map<int,object> regressionObjMap;
static std::map<int,std::map<std::string,object> > regressionObjMap;
static std::map<int,std::string> regressionStackMap;

pythonEnv* pythonEnv::_pyEnv = 0;

std::string GetEnv( const std::string & var ) {
   const char * val = ::getenv( var.c_str() );
   if ( val == 0 ) {
         return "";
   }
   else {
         return val;
   }
}

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
       _runPypredictValue = _regressModule.attr("predictValue");
       _runPyLoadModel = _regressModule.attr("loadModel");

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

 
void pythonEnv::runPython(int trace_id, const std::map<std::string, std::string>& ctxt,
		          const std::map<std::string, std::string>& obs)
{
  if(_pyEnv)
  {
    pythonEnv::handlePickleFiles(trace_id,obs);
    //_runPyProg();
    std::map<std::string, std::string> predictedObs;
    pythonEnv::predictValues(trace_id, ctxt, predictedObs);
    //pythonEnv::anomalyDetection(obs,predictedObs);
  }
  else
  {
    std::cout<<"python env not initialized" << std::endl;
  }
}

void pythonEnv::predictValues(int trace_id,const std::map<std::string, std::string>& ctxt, std::map<std::string, std::string>& predictedObs)
{
     	
     std::map<int,std::map<std::string,object> > :: iterator tracePos = regressionObjMap.find(trace_id);
     if(tracePos == regressionObjMap.end())
     {
	     return;
     }
     std::map<std::string,object> objectMap = tracePos->second;
		
     boost::python::list ctxtList = boost::python::list();
     std::map<std::string,std::string> :: const_iterator ctxtStart = ctxt.begin(), ctxtEnd = ctxt.end();
     for(; ctxtStart != ctxtEnd; ctxtStart++)
     {
        ctxtList.append(ctxtStart->second);
     }
     std::map<std::string,object> :: iterator objectMapStart = objectMap.begin(), objectMapEnd = objectMap.end();
     for(;objectMapStart != objectMapEnd; objectMapStart++)
     {
        _runPypredictValue(objectMapStart->second, ctxtList);
     }



}
void pythonEnv::anomalyDetection(const std::map<std::string, std::string>& obs, std::map<std::string, std::string>& predictedObs)
{
   std::map<std::string, std::string> :: const_iterator obsStart = obs.begin(), obsEnd = obs.end();
   for(; obsStart != obsEnd; obsStart++)
   {
           std::map<std::string, std::string> :: iterator predictedPos = predictedObs.find(obsStart->first);
	   if(predictedPos == predictedObs.end())
	   {
		   continue;
	   }
           float obsF = std::atof((obsStart->second).c_str());
           float predictedObsF = std::atof((predictedPos->first).c_str());
	   float error = (obsF - predictedObsF)/obsF;
	   std::cout << "obs : " << predictedPos->first << "  error : " << error << std::endl; 
    }
}
void pythonEnv::handlePickleFiles(int trace_id,const std::map<std::string, std::string>& obs)
{
	std::string locationPrefix = GetEnv("PICKLE_FILE_PREFIX");
      if(locationPrefix != "")
      {
             locationPrefix += "/";
      }
      std::string stack = regressionStackMap[trace_id];
      locationPrefix += stack;
      std::map<std::string, std::string> :: const_iterator obsStart = obs.begin(), obsEnd = obs.end();
      for(; obsStart != obsEnd; obsStart++)
      {
	      std::string obsName = obsStart->first;
      
	      std::string fullFileLocation  = locationPrefix + "_" + obsName + ".pkl";
        if(regressionObjMap.find(trace_id) == regressionObjMap.end())
        {
          pythonEnv::loadRegressionObjects(fullFileLocation, trace_id);
        }
      }
}


void pythonEnv::recordStack(std::string& stack, int trace_id)
{
   regressionStackMap[trace_id] = stack;
}
void pythonEnv::loadRegressionObjects(std::string& location, int trace_id)
{
   object obj;	
   //std::string filelocation = "/p/lscratchd/mitra3/my_sight_branch/sight/apps/CoEVP/CM/exec/back_dbg.CoEVP/html/widgets/module/data_individual/ModularApp/Constructive_model/TimeStep/Advance_module:measure:PAPI:PAPI_TOT_CYC.pkl";	
   try{	
    std::map<std::string, object> objMap;	
    obj = _runPyLoadModel(location);
    objMap[location] = obj;
    regressionObjMap[trace_id] = objMap; 
   }
   catch(...)
   {
      // If an exception was thrown, translate it to Python
      boost::python::handle_exception();
      std::cout<<"an exception was thrown" << std::endl;
      return;
   }
}

