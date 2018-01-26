#ifndef RIGHT_HAND_SIDE_H
#define RIGHT_HAND_SIDE_H

#include <vector>

typedef std::vector<double> state_type;

class RightHandSide{
  double friction_coef_;
	
public:
  RightHandSide(double friction_coef) : friction_coef_(friction_coef){}
  void operator() (const state_type &x , state_type &dxdt, const double /* t*/);
};

#endif
