#pragma once
#include <pthread.h>
#include <map>
#include <string>
#include "sight_python.h"

class runTimeInformation
{
        public:
           int active_trace_id;
           std::map<std::string, float> active_ctxt;
           std::map<std::string, float> active_obs;
        public:
          runTimeInformation();
          ~runTimeInformation();
          void update(int trace_id, const std::map<std::string, std::string>& ctxt,
                                    const std::map<std::string, std::string>& obs);

};


typedef struct {
   pthread_mutex_t *mutex;
   pthread_cond_t *condition;
} ThreadData;

//runTimeInformation* activeRunTimeInfo;
/* analysis thread */
//pthread_t thread;
//ThreadData threadData;

void initAnalysisThread(pthread_t *t, ThreadData *d);
void* analysisThread(void* param );
