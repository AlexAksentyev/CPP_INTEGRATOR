#include "right_hand_side.h"


void RightHandSide::operator() (const state_type &x,
				state_type &dxdt,
				const double /* t*/){
  dxdt[0] = x[1];
  dxdt[1] = -x[0] - friction_coef_*x[1];
}

