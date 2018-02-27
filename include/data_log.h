// TODO:
// * finish StateList


#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <vector>
#include <fstream>
#include <string>
#include "right_hand_side.h"

class DataLog{
  std::vector<State>& system_state_;
  std::vector<double>& system_position_;
  int reference_pid_;

public:
  DataLog(std::vector<State>& states, std::vector<double>& positions)
    : system_state_(states), system_position_(positions), reference_pid_(0){}
  
  void operator() (const State &state, double position){
    system_state_.push_back(state);
    system_position_.push_back(position);
  }

  void set_reference(int pid){reference_pid_ = pid;}
  
  void write_to_file(std::string name, std::string dir = "../data"); // bin is in build, go out
  void plot(std::string var_name, int pid, std::string line_type="points");
  void plot(std::string var_y_name, std::string var_x_name, int pid, std::string line_type="points");
};

// TO FINISH
// object for creating ensembles of initial conditions
class StateList{


};



#endif // DATA_LOG_H
