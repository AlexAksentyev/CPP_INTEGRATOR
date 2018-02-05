#include "right_hand_side.h"

RightHandSide::RightHandSide(Particle& reference) : particle_(reference) {}


void RightHandSide::operator() (const state_type &x,
				state_type &dxdt,
				const double /* t*/){
  dxdt.col(0) = x.col(1);
  dxdt.col(1) = -x.col(0) - x.col(1);
}

