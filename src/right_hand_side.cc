#include "right_hand_side.h"

RightHandSide::RightHandSide(Particle& reference) : particle_(reference) {}


void RightHandSide::operator() (const state_type &x,
				state_type &dxds,
				const double /* s*/){
  dxds.col(0) = x.col(1);
  dxds.col(1) = -x.col(0) - particle_.G()*x.col(1);
}
