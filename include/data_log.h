// TODO:
// * finish StateList


#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <vector>
#include <fstream>
#include <string>
#include "right_hand_side.h"

struct DataLog{
  std::vector<State>& system_state_;
  std::vector<double>& system_position_;
  
  DataLog(std::vector<State>& states, std::vector<double>& positions)
    : system_state_(states), system_position_(positions){}
  
  void operator() (const State &state, double position){
    system_state_.push_back(state);
    system_position_.push_back(position);
  }

  void write_to_file(std::string name, std::string dir = "../data"); // bin is in build, go out
  void plot(int var_index, int pid, std::string line_type="points");
};

// TO FINISH
// object for creating ensembles of initial conditions
class StateList{


};



#endif // DATA_LOG_H
