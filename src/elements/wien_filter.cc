#include "wien_filter.h"


WienFilterStraight::WienFilterStraight(Particle& reference_particle,
				       double length, double h_gap,
				       double E_hor, double B_vert,
				       std::string name)
  : Element(reference_particle, 0, length, name), h_gap_(h_gap),
    ref_kinetic_energy_(reference_particle.kinetic_energy()),
    kick_voltage_(h_gap*E_hor){

  E_field_base_(0) = E_hor;
  B_field_base_(1) = B_vert;
}

void WienFilterStraight::front_kick(state_type& state){
  for(int i=0; i<state.rows(); i++)
    state(i, 8) -= kick_voltage_*1e-6/ref_kinetic_energy_; // changes dK of state
}

void WienFilterStraight::rear_kick(state_type& state){
 for(int i=0; i<state.rows(); i++)
    state(i, 8) += kick_voltage_*1e-6/ref_kinetic_energy_; // changes dK of state
}
