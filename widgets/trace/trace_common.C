#include <string>
#include <iostream>
#include <sstream>
#include <ostream>
#include <fstream>
#include <assert.h>
#include "attributes_common.h"
#include "trace_common.h"

using namespace std;

namespace sight {
namespace common {


// Returns a string representation of a common::trace::showLocT object
string trace::showLoc2Str(trace::showLocT showLoc) 
{ return (showLoc==trace::showBegin? "showBegin": (showLoc==trace::showEnd? "showEnd": "???")); }

// Returns a string representation of a vizT object
string trace::viz2Str(trace::vizT viz) {
  switch(viz) {
    case trace::table:     return "table";
    case trace::lines:     return "lines";
    case trace::scatter3d: return "scatter3d";
    case trace::decTree:   return "decTree";
    case trace::heatmap:   return "heatmap";
    case trace::boxplot:   return "boxplot";
    default:               return "???";
  }
}

// Returns a string representation of a mergeT object
string trace::mergeT2Str(mergeT merge) {
  switch(merge) {
    case trace::disjMerge: return "disjMerge";
    case trace::avgMerge:  return "avgMerge";
    case trace::maxMerge:  return "maxMerge";
    case trace::minMerge:  return "minMerge";
    default:               return "???";
  }
}

/*******************************************
 ***** Support for parsing trace files *****
 *******************************************/

// Reads the given file of trace observations, calling the provided functor on each instance.
// Each line is a white-space separated sequence of mappings in the format group:key:value, where group
// may be ctxt, obs or anchor. Each call to functor f is provided with a description of a single
// trace file line, with three different maps for each group. For ctxt and obs we map keys to attrValues,
// while for anchor we map keys to integer anchor IDs.

class readTraceFileReader : public attrFileReader {
  public:
  traceFileReader& f;
  public:
  readTraceFileReader(traceFileReader& f): f(f) {}

  void operator()(const std::map<std::string, std::map<std::string, std::string> >& readData, int lineNum) {
/*    cout << "readTraceFileReader: lineNum="<<lineNum<<", readData(#"<<readData.size()<<")="<<endl;
    for(std::map<std::string, std::map<std::string, std::string> >::const_iterator d=readData.begin(); d!=readData.end(); d++) {
      cout << "    "<<d->first<<":"<<endl;
      for(std::map<std::string, std::string>::const_iterator i=d->second.begin(); i!=d->second.end(); i++)
        cout << "        "<<i->first<<" => "<<i->second<<endl;
    }
    cout.flush();*/

    // All trace files must have one or more instances of ctxt and obs and zero or more of anchor
    assert(readData.size()==2 || readData.size()==3);
    assert(readData.find("ctxt") != readData.end());
    assert(readData.find("obs") != readData.end());
    if(readData.size()==3) assert(readData.find("anchor") != readData.end());
    
    // Load and parse the context, observation and anchor portions of the read data
    map<string, attrValue> ctxt;   // The context name=>value mappings
    map<string, attrValue> obs;    // The observation name=>value mappings
    map<string, int>       anchor; // The anchor name=>anchorID mappings
    
    std::map<std::string, std::map<std::string, std::string> >::const_iterator readCtxt    = readData.find("ctxt");
    std::map<std::string, std::map<std::string, std::string> >::const_iterator readObs     = readData.find("obs");
    std::map<std::string, std::map<std::string, std::string> >::const_iterator readAnchor  = readData.find("anchor");
    
    for(std::map<std::string, std::string>::const_iterator c=readCtxt->second.begin();   c!=readCtxt->second.end();   c++)
      ctxt[c->first] = attrValue(c->second, attrValue::unknownT);
    
    for(std::map<std::string, std::string>::const_iterator o=readObs->second.begin();    o!=readObs->second.end();    o++)
      obs[o->first] = attrValue(o->second, attrValue::unknownT);
   
    if(readData.size()==3) 
      for(std::map<std::string, std::string>::const_iterator a=readAnchor->second.begin(); a!=readAnchor->second.end(); a++)
        anchor[a->first] = attrValue::parseInt(a->second);
    
    // Call functor f on this observation
    f(ctxt, obs, anchor, lineNum);
  }
};

// Reads the given file of trace observations, calling the provided functor on each instance.
// Each line is a white-space separated sequence of mappings in the format group:key:value, where group
// may be ctxt, obs or anchor. Each call to functor f is provided with a description of a single
// trace file line, with three different maps for each group. For ctxt and obs we map keys to attrValues,
// while for anchor we map keys to integer anchor IDs.
void readTraceFile(std::string fName, traceFileReader& f) {
  readTraceFileReader reader(f);
  readAttrFile(fName, reader);
}

// Given the context, observables and anchor information for an observation, returns its serialized representation
std::string serializeTraceObservation(const std::map<std::string, attrValue>& ctxt,
                                      const std::map<std::string, attrValue>& obs,
                                      const std::map<std::string, int>&       anchor) {
  ostringstream s;
  
  bool first = true;
  // Emit the average of the window to the output file
  for(map<string, attrValue>::const_iterator c=ctxt.begin(); c!=ctxt.end(); c++) {
    if(!first) s << " "; else first = false;
    escapedStr cName(c->first,  ": ", escapedStr::unescaped);
    escapedStr cVal(c->second.serialize(), ": ", escapedStr::unescaped);
    s << "ctxt:"<<cName.escape()<<":"<<cVal.escape();
  }

  for(map<string, attrValue>::const_iterator o=obs.begin(); o!=obs.end(); o++) {
    if(!first) s << " "; else first = false;
    escapedStr oName(o->first,  ": ", escapedStr::unescaped);
    escapedStr oVal(o->second.serialize(), ": ", escapedStr::unescaped);
    s << "obs:"<<oName.escape()<<":"<<oVal.escape();
  }

  for(map<string, int>::const_iterator a=anchor.begin(); a!=anchor.end(); a++) {
    if(!first) s << " "; else first = false;
    escapedStr aName(a->first,  ": ", escapedStr::unescaped);
    s << "anchor:"<<aName.escape()<<":"<<a->second;
  }
  
  return s.str();
}

//Subrata: trace observer related moved codes:
/*************************
 ***** traceObserver *****
 *************************/

// Records whether we've notified observers of this trace that it has finished
//bool traceObserver::finishNotified=false;

traceObserver::~traceObserver() {
  // Unregister this object from all those that are observing it
  while(observers.size()>0)
    unregisterObserver(observers.begin()->first);

  // Unregister this object from all those that it is observing
  while(observing.size()>0)
    observing.begin()->first->unregisterObserver(this);

  // Notify all the objects that are observing this one that its stream is finished
  notifyObsFinish();
}

// Called when the stream of observations has finished to allow the implementor to perform clean-up tasks.
// This method is optional.
void traceObserver::obsFinished() {
  notifyObsFinish();
}

// Notifies all observers of this trace that it is finished
void traceObserver::notifyObsFinish() {
  // If we have not yet notified all observers that this trace is finished, do so now
  if(!finishNotified) {
    finishNotified=true;
    for(std::map<traceObserver*, int>::iterator o=observers.begin(); o!=observers.end(); o++)
      o->first->obsFinished();
  }
}

// Called from inside observe() to emit an observation that this traceObserver makes.
void traceObserver::emitObservation(int traceID,
                                    const std::map<std::string, std::string>& ctxt,
                                    const std::map<std::string, std::string>& obs,
                                    const std::map<std::string, int>&      obsAnchor) {
  //cout << "traceObserve::emitObservation("<<traceID<<") this="<<this<<", #observers="<<observers.size()<<"=";
  for(std::map<traceObserver*, int>::iterator o=observers.begin(); o!=observers.end(); o++) {
    //cout << o->first << " ";
    o->first->observe(traceID, ctxt, obs, obsAnchor);
  }
  //cout << endl;
}

// Registers/unregisters a given object as an observer of this traceStream
void traceObserver::registerObserver(traceObserver* obs) {
  // If this observer has not been registered until now, initialize it to a count of 1
  if(observers.find(obs) == observers.end()) {
    observers[obs] = 1;
    obs->observing[this] = 1;
    //observersS.insert(obs);
  // Otherwise, increment its count
  } else {
    observers[obs]++;
    obs->observing[this]++;
  }
  //cout << "    "<<this<< " registerObserver("<<obs<<") #observers="<<observers.size()<<endl;
}

void traceObserver::unregisterObserver(traceObserver* obs) {
  assert(observers.find(obs) != observers.end());
  observers[obs]--;
  obs->observing[this]--;
  // If this observer's counter has dropped to 0, erase it
  if(observers[obs] == 0) {
    observers.erase(obs);
    obs->observing.erase(this);
    //observersS.erase(obs);
  }
}

// Places the given set of traceObservers immediately after this observer and before all of the observers that watch it
void traceObserver::prependObservers(const std::set<traceObserver*>& newObservers) {
  // Back up the original set of observers and remove them
  map<traceObserver*, int> origObservers = observers;
  while(observers.size()>0) {
// newObservers and origObservers must be disjoint
    assert(newObservers.find(observers.begin()->first) == newObservers.end());

    unregisterObserver(observers.begin()->first);
  }

  // Add each new observer to this observer
  for(set<traceObserver*>::const_iterator n=newObservers.begin(); n!=newObservers.end(); n++) {
    registerObserver(*n);
    // And add all of the original observers to watch this current new observer
    for(map<traceObserver*, int>::iterator o=origObservers.begin(); o!=origObservers.end(); o++)
      (*n)->registerObserver(o->first);
  }
}

/******************************
 ***** traceObserverQueue *****
 ******************************/

traceObserverQueue::traceObserverQueue() {
  firstO = NULL;
  lastO = NULL;
}

traceObserverQueue::traceObserverQueue(const std::list<traceObserver*>& observersL)/* : queue(observersL)*/ {
//cout << "traceObserverQueue::traceObserverQueue() #observersL.size="<<observersL.size()<<endl;
  // If observersL is non-empty
  if(observersL.size()>0) {
    firstO = observersL.front();
   lastO  = observersL.back();

    // Iterate through the observersL list, adding each traceObserver in the list as an observer of the traceObserver
    // that precedes it.

    list<traceObserver*>::const_iterator o=observersL.begin();

    // The traceObserver most recently encountered by the loop
    traceObserver* recentO=*o;

//    cout << "  firstO="<<firstO<<", lastO="<<lastO<<", queue="<<this<<endl;

    for(o++; o!=observersL.end(); o++) {
//      cout << "    traceObserverQueue recentO="<<recentO<<" <-- *o="<<*o<<endl;
      recentO->registerObserver(*o);
      recentO = *o;
    }
  // If observersL is empty
  } else {
    firstO = NULL;
    lastO = NULL;
  }
}

// Push a new observer to the back of the observers queue
void traceObserverQueue::push_back(traceObserver* obs)
{
  //cout << "queue::push_back(obs="<<obs<<") firstO="<<firstO<<endl;
  // If this is the first element in the observers queue
  if(firstO == NULL) {
   firstO = obs;
    lastO  = obs;

    // Take any observers currently registered with this queue and register them with the queue's last element
    for(map<traceObserver*, int>::iterator o=observers.begin(); o!=observers.end(); o++)
      lastO->registerObserver(o->first);

  // If observers already has elements
  } else {
    //cout << "#observers="<<observers.size()<<endl;
    // Move over any observers currently registered with this queue to be registered with the queue's new last element
    for(map<traceObserver*, int>::iterator o=observers.begin(); o!=observers.end(); o++) {
      o->first->unregisterObserver(lastO);
      o->first->registerObserver(obs);
    }

    // Place obs immediately after lastO
    lastO->registerObserver(obs);

    //cout << "lastO="<<lastO<<", #lastO->observers="<<lastO->observers.size()<<endl;
    lastO = obs;
  }
}
// Push a new observer to the front of the observers queue
void traceObserverQueue::push_front(traceObserver* obs)
{
  //queue.push_front(obs);
  // If this is the first element in observers
  if(lastO==NULL) {
    firstO = obs;
    lastO  = obs;

    // Take any observers currently registered with this queue and register them with the queue's last element
    for(map<traceObserver*, int>::iterator o=observers.begin(); o!=observers.end(); o++)
      lastO->registerObserver(o->first);
  // If observers already has elements
  } else {
    // Place obs immediately before firstO
    obs->registerObserver(firstO);

    firstO = obs;
  }
}

// Override the registration methods from traceObserver to add the observers to the back of the queue
// Registers/unregisters a given object as an observer of this traceStream
void traceObserverQueue::registerObserver(traceObserver* obs) {
  // Register this observer within this object. We'll use traceObserver::observers as the record of which
  // traceObservers are registered and we'll make sure that they're registered with the last element in the queue.
  traceObserver::registerObserver(obs);

  // If there are currently elements in the observers queue
  if(lastO!=NULL) {
   // Also register this observer within the last element. It must be that the set of observers registered with
    // this traceObserverQueue is identical to the set of observers registered with the queue's last element.
    lastO->registerObserver(obs);
  }
}

void traceObserverQueue::unregisterObserver(traceObserver* obs) {
  // Unregister this observer within this object. We'll use traceObserver::observers as the record of which
  // traceObservers are registered and we'll make sure that they're registered with the last element in the queue.
  traceObserver::unregisterObserver(obs);

  // If there are currently elements in the observers queue
  if(lastO!=NULL) {
    // Also unregister this observer within the last element. It must be that the set of observers registered with
    // this traceObserverQueue is identical to the set of observers registered with the queue's last element.
    lastO->unregisterObserver(obs);
  }
}

// traceID - unique ID of the trace from which the observation came
// ctxt - maps the names of the observation's context attributes to string representations of their values
// obs - maps the names of the trace observation attributes to string representations of their values
// obsAnchor - maps the names of the trace observation attributes to the anchor that identifies where they were observed
void traceObserverQueue::observe(int traceID,
                                 const std::map<std::string, std::string>& ctxt,
                                 const std::map<std::string, std::string>& obs,
                                 const std::map<std::string, int>&      obsAnchor) {
  if(firstO!=NULL)
    // Forward the observation to the first traceObserver in the queue and let it forward it to its
    // successors in the queue as it needs to.
 firstO->observe(traceID, ctxt, obs, obsAnchor);

  /*
  static bool observing=false;
  static list<traceObserver*>::iterator curObserver;
  static int obsIdx;
  // Points
  //static const std::set<traceObserver*>* startObservers;

  // If this is the start of an observation
  traceObserver* o=NULL;
  if(observing==false) {
    curObserver = queue.begin();
    o = *curObserver;
    observing = true;
    obsIdx=0;
    //cout << "traceObserverQueue::observe("<<traceID<<") start obsIdx="<<obsIdx<<endl;
  // We're in the middle of observing
  } else {
    o = *curObserver;

    obsIdx++;

    //cout << "traceObserverQueue::observe("<<traceID<<") continue obsIdx="<<obsIdx<<endl;
  }

  curObserver++;
  // If this is the last traceObserver in observers, reset state
if(curObserver == queue.end()) {
    observing = false;
    //cout << "traceObserverQueue::observe("<<traceID<<") end obsIdx="<<obsIdx<<endl;
  }

  std::set<traceObserver*> nextObs; nextObs.insert(this);
  o->observe(traceID, ctxt, obs, obsAnchor, nextObs);
  //emitObservation(traceID, ctxt, obs, obsAnchor, observers);*/
}

// Called when the stream of observations has finished to allow the implementor to perform clean-up tasks.
// This method is optional.
void traceObserverQueue::obsFinished() {
  if(firstO!=NULL)
    // Forward the completion notification to the first traceObserver in the queue and let it forward it to its
    // successors in the queue as it needs to.
    firstO->obsFinished();
}


}; // namespace common
}; // namespace sight
