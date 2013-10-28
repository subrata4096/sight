#pragma once

#include <list>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../sight_common.h"
#include "../sight_layout.h"

namespace sight {
namespace layout {

class scopeLayoutHandlerInstantiator {
  public:
  scopeLayoutHandlerInstantiator();
};
extern scopeLayoutHandlerInstantiator scopeLayoutHandlerInstance;

class scope: public block, public common::scope
{
  static std::vector<std::string> colors;
  static int colorIdx; // The current index into the list of colors 
  
  public:
  scopeLevel level;

  // properties: maps property names to their values
  // level - the type of visualization used, with higher levels associated with more amounts of debug output
  //    There are several features that are enabled by the levels:
  //       own_file: Text inside the scope is written to a separate file. Users must manually click a button to see it.
  //       own_color: The background color of the scope is different from its parent scope.
  //       label_shown: The label of this scope is shown in a larger font, along with controls to minimize the scope 
  //              by clicking on the label and open GDB to the point in the execution where the scope started
  //    Different levels
  //    high: own_file, own_color, label_shown
  //    medium: own_color, label_shown
  //    low: label_shown
  //    min: none of the above
  scope(properties::iterator props);
  
  private:
  // Common initialization code
  void init(scopeLevel level);
  
  public:
    
  // Called to notify this block that a sub-block was started/completed inside of it. 
  // Returns true of this notification should be propagated to the blocks 
  // that contain this block and false otherwise.
  bool subBlockEnterNotify(block* subBlock) { return true; }
  bool subBlockExitNotify (block* subBlock) { return true; }
  
  // Called to enable the block to print its entry and exit text
  void printEntry(std::string loadCmd);
  void printExit();
  
  ~scope();
}; // scope

/*class workscope: public scope
{
  class DiffFunctor {
    double operator()(void* data1, void* data2);
  };

  class dataItem {
    std::string name;
    void* data;
    DiffFunctor* diff;
  };
    
  std::set<dataItem> data;

  public:
  workscope(std::string label,                                   scopeLevel level, const attrOp& onoffOp);
  workscope(std::string label, const anchor& pointsTo,           scopeLevel level, const attrOp& onoffOp);
  workscope(std::string label, const std::set<anchor>& pointsTo, scopeLevel level, const attrOp& onoffOp);
  workscope(std::string label,                                                     const attrOp& onoffOp);
  workscope(std::string label, const anchor& pointsTo,                             const attrOp& onoffOp);
  workscope(std::string label, const std::set<anchor>& pointsTo,                   const attrOp& onoffOp);
  workscope(std::string label,                                   scopeLevel level=medium);
  workscope(std::string label, const anchor& pointsTo,           scopeLevel level=medium);
  workscope(std::string label, const std::set<anchor>& pointsTo, scopeLevel level=medium);

  ~workscope();

  void data(std::string name, void* data, DiffFunctor* diff);
};*/

}; // namespace layout
}; // namespace sight