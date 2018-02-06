
#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <vector>
#include "right_hand_side.h"

struct DataLog{
  std::vector<state_type>& system_state_;
  std::vector<double>& system_position_;
  
  DataLog(std::vector<state_type>& states, std::vector<double>& positions)
  : system_state_(states), system_position_(positions){}
  
  void operator() (const state_type &state, double position){
    system_state_.push_back(state);
    system_position_.push_back(position);
  }
};

#endif // DATA_LOG_H
