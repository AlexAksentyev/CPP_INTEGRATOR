#include "right_hand_side.h"

RightHandSide::RightHandSide(Particle& reference) : particle_(reference) {}


void RightHandSide::operator() (const state_type &state,
				state_type &derivative,
				const double /* s*/){

  int state_num = state.rows();


  double P0c = particle_.Pc();
  variable_col Px = state.col(6)*P0c;
  variable_col Py = state.col(7)*P0c;
  variable_col Pc = particle_.Pc(dK);
  variable_col Ps = sqrt(Pc*Pc - Px*Px - Py*Py);

  variable_col dK = state.col(8);

  variable_col kin_energy = particle_.kinetic_energy(dK);

  
}
