#pragma once

#include <map>
//#include "../../sight_layout.h"

namespace sight {

namespace common {
class trace {
  public:
  // Identifies the type of visualization used to show the trace
  typedef enum {table, lines, scatter3d, decTree, heatmap, boxplot, unknown} vizT;

  // Indicates whether the trace visualization should be shown at the beginning or the end of its visual block
  typedef enum {showBegin, showEnd} showLocT;
  
  // Returns a string representation of a showLocT object
  static std::string showLoc2Str(showLocT showLoc);
    
  // The way trace observations from multiple stream are combined together
  typedef enum {disjMerge, // Observations with the same context from different streams are differentiated by the stream's ID
                // Observations with the same context from different streams are
                avgMerge,  // averaged
                maxMerge,  // max-ed
                minMerge}  // min-ed
            mergeT;
  
  // Returns a string representation of a mergeT object
  static std::string mergeT2Str(mergeT merge);
  
  // Returns a string representation of a vizT object
  static std::string viz2Str(vizT viz);
};

/*******************************************
 ***** Support for parsing trace files *****
 *******************************************/

// Type of the user-provided functor that takes as input a description of the observation data read from traces.
// readData: Maps field group (e.g. "ctxt", "obs", "anchor") to the mapping of attribute names to their string representations
// lineNum: The current line in the file, which is useful for generating error messages.
class traceFileReader {
  public:
  virtual void operator()(const std::map<std::string, attrValue>& ctxt,
                          const std::map<std::string, attrValue>& obs,
                          const std::map<std::string, int>& anchor, 
                          int lineNum)=0;
}; 

// Reads the given file of trace observations, calling the provided functor on each instance.
// Each line is a white-space separated sequence of mappings in the format group:key:value, where group
// may be ctxt, obs or anchor. Each call to functor f is provided with a description of a single
// trace file line, with three different maps for each group. For ctxt and obs we map keys to attrValues,
// while for anchor we map keys to integer anchor IDs.
void readTraceFile(std::string fName, traceFileReader& f);


// Given the context, observables and anchor information for an observation, returns its serialized representation
std::string serializeTraceObservation(const std::map<std::string, attrValue>& ctxt,
                                      const std::map<std::string, attrValue>& obs,
                                      const std::map<std::string, int>& anchor);

//subrata: make anchor as int for traceobserver


//subrata: change: moving traceobserver from layout to common as we will use it in structure as well
// Interface implemented by objects that listen for observations a traceStream reads. traceObservers may be arranged
// as arbitrary graphs of filters, consuming observations from some traceStreams or traceObservers and forwarding
// the same or other observations (e.g. the sum of multiple observations) to other traceObservers. For simplicity
// traceStreams also implement the traceObserver interface so that they can leverage the common mechanism for
// forwarding observations.

class traceObserver {
  //protected:
public:
  // Set of objects this traceObserver informs of any observations it reads. We map each traceObserver to the number
  // of times it was registered to allow a given observer to get registered multiple times.
  std::map<traceObserver*, int> observers;
  //std::set<traceObserver*> observersS;

  // Set of objects being observed by this observer
  std::map<traceObserver*, int> observing;

  // Records whether we've notified observers of this trace that it has finished
  bool finishNotified;

  public:

  traceObserver() { finishNotified=false; }
  virtual ~traceObserver();
  // Called on each observation from the traceObserver this object is observing
  // traceID - unique ID of the trace from which the observation came
  // ctxt - maps the names of the observation's context attributes to string representations of their values
  // obs - maps the names of the trace observation attributes to string representations of their values
  // obsAnchor - maps the names of the trace observation attributes to the anchor that identifies where they were observed
  // follower - if non-NULL points to the traceObserver object to which this observe call should pass on
  //    the observations it emits.

virtual void observe(int traceID,
               const std::map<std::string, std::string>& ctxt,
               const std::map<std::string, std::string>& obs,
               const std::map<std::string, int>&      obsAnchor)=0;
/*  { observe(traceID, ctxt, obs, obsAnchor, std::set<traceObserver*>()); }

  virtual void observe(int traceID,
                       const std::map<std::string, std::string>& ctxt,
                       const std::map<std::string, std::string>& obs,
                       const std::map<std::string, anchor>&      obsAnchor,
                       const std::set<traceObserver*>&           observers)=0;*/

  // Called when the stream of observations has finished to allow the implementor to perform clean-up tasks.
  // This method is optional.
  virtual void obsFinished();

  // Notifies all observers of this trace that it is finished
  void notifyObsFinish();

  // Called from inside observe() to emit an observation that this traceObserver makes.
  void emitObservation(int traceID,
                       const std::map<std::string, std::string>& ctxt,
                       const std::map<std::string, std::string>& obs,
                       const std::map<std::string, int>&      obsAnchor);/* {
    emitObservation(traceID, ctxt, obs, obsAnchor, observersS);
  }

  // Emits an empty observation for cases where the caller wishes that observers are informed that
  // an observation was made but has not information to provide
  / *void emitEmptyObservation(int traceID, const std::set<traceObserver*>& observers) {
    emitObservation(traceID,
                    std::map<std::string, std::string>(),
                    std::map<std::string, std::string>(),
                    std::map<std::string, anchor>(),
                    observersS);
  }* /

   void emitObservation(int traceID,
                       const std::map<std::string, std::string>& ctxt,
                       const std::map<std::string, std::string>& obs,
                       const std::map<std::string, anchor>&      obsAnchor,
                       const std::set<traceObserver*>&           observers);*/

  // Registers/unregisters a given object as an observer of this traceStream
  virtual void registerObserver(traceObserver* obs);
  virtual void unregisterObserver(traceObserver* obs);

  // Places the given set of traceObservers immediately after this observer and before all of the observers that watch it
  virtual void prependObservers(const std::set<traceObserver*>& newObservers);

  // Returns the total number of observers for this object
  int numObservers() const { return observers.size(); }
}; // traceObserver

typedef common::easylist<traceObserver* > traceObservers;

// Maintains a chain of observers, each of which receives observations, filters them and then passes them on to
// observers that follow it in the chain. An observer is not required to pass each observation it receives onwards
// and may send out multiple observations in a single observe() call.
class traceObserverQueue: public traceObserver {
  protected:
  // The first traceObserver in the observers queue
  traceObserver* firstO;

  // The last traceObserver in the observers queue
  traceObserver* lastO;
  //std::list<traceObserver*> queue;

  public:
  traceObserverQueue();
  traceObserverQueue(const std::list<traceObserver*>& observersL);

  // Push a new observer to the back of the observers queue
  void push_back(traceObserver* obs);

  // Push a new observer to the front of the observers queue
  void push_front(traceObserver* obs);

  // Override the registration methods from traceObserver to add the observers to the back of the queue
  // Registers/unregisters a given object as an observer of this traceStream
  void registerObserver(traceObserver* obs);
  void unregisterObserver(traceObserver* obs);
	
  
  // traceID - unique ID of the trace from which the observation came
  // ctxt - maps the names of the observation's context attributes to string representations of their values
  // obs - maps the names of the trace observation attributes to string representations of their values
  // obsAnchor - maps the names of the trace observation attributes to the anchor that identifies where they were observed
  void observe(int traceID,
               const std::map<std::string, std::string>& ctxt,
               const std::map<std::string, std::string>& obs,
               const std::map<std::string, int>&      obsAnchor);

  // Called when the stream of observations has finished to allow the implementor to perform clean-up tasks.
  // This method is optional.
  void obsFinished();
}; // traceObserverQueue


} // namespace common
} // namespace sight
