#pragma once
//#include <boost/python.hpp>
//using namespace boost;
//using namespace boost::python;
#include <map>
#include <string>
class pythonEnv
{
	/*
	private:
		object* _sys;
		object* _regressModule;
		object* _runPyProg;
		*/
	private:
		static pythonEnv* _pyEnv;
	public:
	pythonEnv();
	~pythonEnv();
	static void init();
	static void exit();
	static void runPython( const std::map<std::string, std::string>& ctxt,
			       const std::map<std::string, std::string>& obs);

	static void loadRegressionObjects(std::string& location, int trace_id);
};
