#include <iostream>
#include <string>
#include <boost/python.hpp>
//#include "Python.h"
#include "sight_python.h"
#include "sight_analysis_thread.h"

extern runTimeInformation* activeRunTimeInfo;
///* analysis thread */
extern pthread_t thread;
extern ThreadData threadData;

runTimeInformation::runTimeInformation()
{
	this->active_trace_id = -999;
}
runTimeInformation::~runTimeInformation()
{
	this->active_ctxt.clear();
	this->active_obs.clear();
}

void runTimeInformation::update(int trace_id, const std::map<std::string, std::string>& ctxt,
		                                    const std::map<std::string, std::string>& obs)
{
//mutex lock
	this->active_ctxt.clear();
	this->active_obs.clear();

	this->active_trace_id = trace_id;

        std::map<std::string,std::string> :: const_iterator ctxtStart = ctxt.begin(), ctxtEnd = ctxt.end();
        for(; ctxtStart != ctxtEnd; ctxtStart++)
        {

          this->active_ctxt[ctxtStart->first] = std::atof((ctxtStart->second).c_str());
        }
        std::map<std::string,std::string> :: const_iterator obsStart = obs.begin(), obsEnd = obs.end();
        for(;obsStart != obsEnd; obsStart++)
        {
          this->active_obs[obsStart->first] = std::atof((obsStart->second).c_str());
        }

	std::cout << "In update thread: " << "trace id: " << trace_id << "\n" << this->active_obs["module:measure:PAPI:PAPI_TOT_CYC"] << std::endl;
//end mutex lock
}


using namespace boost::python;
static object _sys;
static object _regressModule;
static object _runPyProg;
static object _runPypredictValue;
static object _runPygetValidRangeOfValuesWithErrorAdjustmentcreateDataPoint;
static object _runPyLoadModel;
static object _runPyLoadAnomalyDetectionEngine;


static object ADE;
//static std::map<int,object> regressionObjMap;
//static std::map<int,std::map<std::string,object> > regressionObjMap;
static std::map<int,object> regressionObjMap;
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

std::string pythonEnv::locationPrefix = "";

pythonEnv::pythonEnv()
{
	std::cout << "creating pythonEnv" << std::endl;
	pythonEnv::locationPrefix = GetEnv("PICKLE_FILE_PREFIX");
	if(pythonEnv::locationPrefix != "")
        {
             pythonEnv::locationPrefix += "/";
        }

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
       _runPygetValidRangeOfValuesWithErrorAdjustmentcreateDataPoint = _regressModule.attr("getValidRangeOfValuesWithErrorAdjustment");
       //_runPyLoadModel = _regressModule.attr("loadModel");
       _runPyLoadModel = _regressModule.attr("printModule");
       _runPyLoadAnomalyDetectionEngine = _regressModule.attr("loadAnomalyDetectionEngine");
       pythonEnv::loadAnomalyDetectionEngine(pythonEnv::locationPrefix);
     } catch( error_already_set ) {
       PyErr_Print();
    }
 
}

void pythonEnv::loadAnomalyDetectionEngine(std::string& a)
{
	try{
         //load the Anomaly Detection Engine
	 ADE = _runPyLoadAnomalyDetectionEngine(pythonEnv::locationPrefix);
	}
	catch( ...)
	{
		std::cout << "\n ERROR: could not load anomaly detection engine " << std::endl; 
	}
}

static object getAnomalyDetectionObject(std::string& fileName)
{
      object aDetectObj = ADE.attr("getAnomalyDetectionObject")(fileName);
      return aDetectObj;	
}

static void getValidRangeOfValues(const std::map<std::string,float> & ctxt,
                                 object& anoDetectObj,std::string& targetName,std::pair<float,float>& valRange)
{
  boost::python::dict ctxtDict = boost::python::dict();
  std::map<std::string,float> :: const_iterator ctxtStart = ctxt.begin(), ctxtEnd = ctxt.end();
  for(; ctxtStart != ctxtEnd; ctxtStart++)
  {

        //ctxtList.append(std::atof((ctxtStart->second).c_str()));
        ctxtDict[ctxtStart->first] = ctxtStart->second;
  }	
  boost::python::tuple rangeTuple = boost::python::extract<boost::python::tuple>(_runPygetValidRangeOfValuesWithErrorAdjustmentcreateDataPoint(ctxtDict,anoDetectObj,targetName));
  float lowerRange = boost::python::extract<float>(rangeTuple[0]);
  float uperRange = boost::python::extract<float>(rangeTuple[1]);

  valRange = std::make_pair(lowerRange,uperRange);
  return;
   
}
pythonEnv::~pythonEnv()
{
  Py_Finalize();
}
void pythonEnv::init()
{
   _pyEnv = new pythonEnv();
   //initialize the following global variable
   activeRunTimeInfo = new runTimeInformation();

   // Init analysis thread
   initAnalysisThread(&thread, &threadData);
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

void pythonEnv::signalCondition()
{
  //assert(pthread_mutex_lock(threadData.mutex) == 0);
  pthread_cond_signal(threadData.condition);
  //assert(pthread_mutex_unlock(threadData.mutex) == 0);
}

void pythonEnv::runPython(int trace_id, const std::map<std::string, std::string>& ctxt,
		          const std::map<std::string, std::string>& obs)
{
  if(_pyEnv)
  {
    //update the info about current runtime. Based on this another "analysisThread" will work in parallel
    
    assert(pthread_mutex_lock(threadData.mutex) == 0); // Acquire lock
    
    activeRunTimeInfo->update(trace_id,ctxt,obs);
    
    assert(pthread_mutex_unlock(threadData.mutex) == 0); // Release lock 
 
    signalCondition();

    //pythonEnv::handlePickleFiles(trace_id,obs);
    //_runPyProg();
    //std::map<std::string, double> predictedObs;
    //pythonEnv::predictValues(trace_id, ctxt, predictedObs);
    //pythonEnv::anomalyDetection(obs,predictedObs);
  }
  else
  {
    std::cout<<"python env not initialized" << std::endl;
  }
}

void pythonEnv::predictValues(int trace_id,const std::map<std::string, float>& ctxt, const std::map<std::string, float>& obs, std::map<std::string, std::pair<float,float> >& predictedObs)
{
   //std::map<std::string,float> dtPt;
   //dtPt["A"] = 1.0;
   //dtPt["ABC"] = 1.98;
   //dtPt["ADD"] = 0.023;

   try
   {  	
     std::map<int,object> :: iterator tracePos = regressionObjMap.find(trace_id);
     if(tracePos == regressionObjMap.end())
     {
	     return;
     }
     object anomalyDetectObject = tracePos->second;
     #if 0
     boost::python::list ctxtList = boost::python::list();
     std::map<std::string,float> :: const_iterator ctxtStart = ctxt.begin(), ctxtEnd = ctxt.end();
     for(; ctxtStart != ctxtEnd; ctxtStart++)
     {
	     
        //ctxtList.append(std::atof((ctxtStart->second).c_str()));
        ctxtList.append(ctxtStart->second);
     }
     std::map<std::string,object> :: iterator objectMapStart = objectMap.begin(), objectMapEnd = objectMap.end();
     for(;objectMapStart != objectMapEnd; objectMapStart++)
     {
        //predictedObs[objectMapStart->first] = boost::python::extract<float>(_runPypredictValue(objectMapStart->second, ctxtList));
	float lowerRange = -999;
	float upperRange = -999;
        pythonEnv::getValidRangeOfValues(ctxt,anomalyObject,lowerRange,upperRange);
	//predictedObs[objectMapStart->first] = boost::python::extract<float>(_runPypredictValue(objectMapStart->second, ctxtList));
     }
     #endif
     std::map<std::string,float> :: const_iterator obsStart = obs.begin(), obsEnd = obs.end();
     for(;obsStart != obsEnd; obsStart++)
     {
        //predictedObs[objectMapStart->first] = boost::python::extract<float>(_runPypredictValue(objectMapStart->second, ctxtList));
        std::pair <float,float> valueRange;
        std::string targetName = obsStart->first;	
        getValidRangeOfValues(ctxt,anomalyDetectObject,targetName,valueRange);
        predictedObs[obsStart->first] = valueRange;
     }
   }
     catch(...)
   {
      // If an exception was thrown, translate it to Python
      boost::python::handle_exception();
      std::cout<<"During prediction: an exception was thrown" << std::endl;
      return;
   }

}
void pythonEnv::anomalyDetection(const std::map<std::string, float>& obs, std::map<std::string, std::pair<float,float> >& predictedObs)
{
#if 0
   std::map<std::string, float> :: const_iterator obsStart = obs.begin(), obsEnd = obs.end();
   for(; obsStart != obsEnd; obsStart++)
   {
           std::map<std::string, float> :: iterator predictedPos = predictedObs.find(obsStart->first);
	   if(predictedPos == predictedObs.end())
	   {
		   continue;
	   }
           //float obsF = std::atof((obsStart->second).c_str());
           float obsF = obsStart->second;
           //float predictedObsF = std::atof((predictedPos->first).c_str());
           float predictedObsF = std::atof((predictedPos->first).c_str());
	   float error = (obsF - predictedObsF)/obsF;
	   std::cout << "obs : " << predictedPos->first << "  error : " << error << std::endl; 
    }
#endif
   std::map<std::string, float> :: const_iterator obsStart = obs.begin(), obsEnd = obs.end();
   for(; obsStart != obsEnd; obsStart++)
   {
           std::map<std::string, std::pair<float,float> > :: iterator predictedPos = predictedObs.find(obsStart->first);
           if(predictedPos == predictedObs.end())
           {
                   continue;
           }
           std::cout << "\n actual and predicted values:" << obsStart->second <<" --- " << (predictedPos->second).first << " , " << (predictedPos->second).second << std::endl; 
           if(!((obsStart->second >= (predictedPos->second).first) && (obsStart->second <= (predictedPos->second).second)))
	   {
               std::cout <<"\n\n\n THERE is an anomaly!! \n";
           }
   }
}
void pythonEnv::handlePickleFiles(int trace_id/*,const std::map<std::string, float>& obs*/)
{
      #if 0
      //std::string stack = regressionStackMap[trace_id];
      std::map<std::string, float> :: const_iterator obsStart = obs.begin(), obsEnd = obs.end();
      for(; obsStart != obsEnd; obsStart++)
      {
	std::string obsName = obsStart->first;
      
        if(regressionObjMap.find(trace_id) == regressionObjMap.end())
        {
          pythonEnv::loadRegressionObjects(obsName, trace_id);
        }
      }
      #endif
      if(regressionObjMap.find(trace_id) == regressionObjMap.end())
        {
          pythonEnv::loadRegressionObjects(trace_id);
        }
}


void pythonEnv::recordStack(std::string& stack, int trace_id)
{
   regressionStackMap[trace_id] = stack;
}

void pythonEnv::loadRegressionObjects(/*std::string& obsName,*/ int trace_id)
{
   //std::cout << "\n--> OBSERVATION NAME : " << obsName << " trace id : " << trace_id << std::endl;	
	
   object obj;	
   //std::string fileLoc = locationPrefix + regressionStackMap[trace_id];
   std::string fileName = regressionStackMap[trace_id];
   #if 0
   std::string fullFileLocation  = fileLoc + "_" + obsName + ".cpkl";
   //std::string filelocation = "/p/lscratchd/mitra3/my_sight_branch/sight/apps/CoEVP/CM/exec/back_dbg.CoEVP/html/widgets/module/data_individual/ModularApp/Constructive_model/TimeStep/Advance_module:measure:PAPI:PAPI_TOT_CYC.pkl";	
   try{	
    std::map<std::string, object> objMap;	
    obj = _runPyLoadModel(fullFileLocation);
    objMap[obsName] = obj;
    regressionObjMap[trace_id] = objMap; 
   }
   catch(...)
   {
      // If an exception was thrown, translate it to Python
      boost::python::handle_exception();
      std::cout<<"an exception was thrown during loading of: " << fullFileLocation << std::endl;
      return;
   }
   #endif
   //std::string fullFileLocation  = locationPrefix + "_" + obsName + ".tsv";
   std::string fullFileLocation  = fileName + ".tsv";
   std::cout << "\nloadRegressionObjects: fileName = " << fullFileLocation << std::endl; 
   try{
       obj = getAnomalyDetectionObject(fullFileLocation);
       regressionObjMap[trace_id] = obj;
   }
   catch(...)
   {
       // If an exception was thrown, translate it to Python
      boost::python::handle_exception();
      std::cout<<"an exception was thrown during loading of: " << fullFileLocation << std::endl;
      return;
   }
}

