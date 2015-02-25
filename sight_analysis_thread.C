#include <iostream>
#include <cstdlib>
#include <cassert>
#include "sight_analysis_thread.h"

runTimeInformation* activeRunTimeInfo;
/* analysis thread */
pthread_t thread;
ThreadData threadData;

//extern runTimeInformation* activeRunTimeInfo;

void initAnalysisThread(pthread_t *t, ThreadData *d)
{
  // Initialize mutex and condition variable
  d->mutex = (pthread_mutex_t *)malloc(sizeof (pthread_mutex_t));
  pthread_mutex_init(d->mutex, NULL);
  d->condition = (pthread_cond_t *)malloc(sizeof (pthread_cond_t));
  pthread_cond_init(d->condition, NULL);
  // Initialize analysis thread.
  int ret = pthread_create(t, NULL, analysisThread, static_cast<void *>(d));
  //int ret = pthread_create(t, NULL, analysisThread, d);
}

void * analysisThread(void* param)
{
  ThreadData *threadData = static_cast<ThreadData* >(param);
  std::cout << "Analysis thread started" <<std::endl;
  while(1)
  {

          
	  //assert(pthread_mutex_lock(threadData->mutex) == 0); // Acquire lock
	  assert(pthread_mutex_lock(threadData->mutex) == 0); // Acquire lock
          
	  int ret  = pthread_cond_wait(threadData->condition,threadData->mutex);
          
          int current_trace_id = activeRunTimeInfo->active_trace_id;
          std::map<std::string, float> current_ctxt = activeRunTimeInfo->active_ctxt;
          std::map<std::string, float> current_obs = activeRunTimeInfo->active_obs;
       
	  assert(pthread_mutex_unlock(threadData->mutex) == 0); // Release lock
          
	  std::cout << "From analysis thread: " << "trace id: " << current_trace_id << "\n" << current_obs["module:measure:PAPI:PAPI_TOT_CYC"] << std::endl;

          //pythonEnv::handlePickleFiles(current_trace_id,current_obs);
          pythonEnv::handlePickleFiles(current_trace_id);
          std::map<std::string, std::pair<float,float> > predictedObs;
          pythonEnv::predictValues(current_trace_id, current_ctxt,current_obs, predictedObs);
          pythonEnv::anomalyDetection(current_obs,predictedObs); 
	   

  }

  pthread_exit(NULL);

}
