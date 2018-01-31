#include "right_hand_side.h"

RightHandSide::RightHandSide(Element_ptr host,
			     Particle_ptr reference){

  element_ = host;
  particle_ = reference;
}


void RightHandSide::operator() (const state_type &x,
				state_type &dxdt,
				const double /* t*/){
  dxdt[0] = x[1];
  dxdt[1] = -x[0] - x[1];
}

