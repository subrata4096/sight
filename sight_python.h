#pragma once
//#include <boost/python.hpp>
//using namespace boost;
//using namespace boost::python;
#include <map>
#include <string>
//#include "sight_analysis_thread.h"

//extern runTimeInformation* activeRunTimeInfo;
/* analysis thread */
//extern pthread_t thread;
//extern ThreadData threadData;

class pythonEnv
{
	/*
	private:
		object* _sys;
		object* _regressModule;
		object* _runPyProg;
		*/
	public:
	        static std::string locationPrefix;
	private:
		static pythonEnv* _pyEnv;
	public:
	pythonEnv();
	~pythonEnv();
	static void init();
	static void exit();
	static void signalCondition();
	static void runPython(int trace_id, const std::map<std::string, std::string>& ctxt,
			                          const std::map<std::string, std::string>& obs);
	static void loadRegressionObjects(std::string& obsName, int trace_id);
	static void recordStack(std::string& stack, int trace_id);
	static void predictValues(int trace_id,const std::map<std::string, float>& ctxt, std::map<std::string, float>& predictedObs);
	static void handlePickleFiles(int trace_id,const std::map<std::string, float>& obs);
	static void anomalyDetection(const std::map<std::string, float>& obs, 
			         std::map<std::string, float>& predictedObs);
};
