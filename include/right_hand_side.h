// The Right Hand Side of the differential equation
// corresponding to a particular element
// has a pointer to the element for the computation of
// the EM fields


#ifndef RIGHT_HAND_SIDE_H
#define RIGHT_HAND_SIDE_H

#include <vector>
#include "particle.h"
#include <Eigen/Dense>

static const int VAR_NUM = 12;
// variable names
/*   x, y, s, */
/*   t, Theta, H, */
/*   px, py, dK, */
/*   Sx, Sy, Sz */ 

using state_type = Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;

class RightHandSide{
  Particle& particle_;
	
public:
  RightHandSide(Particle& reference_particle);
  void operator() (const state_type &x , state_type &dxds, const double /* s*/);
};


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



#endif
