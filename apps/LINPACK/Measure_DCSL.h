#ifndef  __MEASURE_DCSL__
#define  __MEASURE_DCSL__


namedMeasures getMeasures();
/**
 * {

   return namedMeasures(
#ifdef RAPL
                              "RAPL", new RAPLMeasure()
#else
                              "time", new timeMeasure(),
                              "PAPI", new PAPIMeasure(papiEvents(PAPI_TOT_INS, PAPI_TOT_CYC))
#endif
                             );

}
**/


#endif

