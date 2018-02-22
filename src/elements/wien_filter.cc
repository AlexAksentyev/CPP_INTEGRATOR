#include "wien_filter.h"
#include "particle.h"
#include <math.h>

WFStraight::WFStraight(Particle& reference_particle,
		       double length, double h_gap,
		       double E_hor, double B_vert,
		       std::string name)
  : Element(reference_particle, 0, length, name), //h_gap_(h_gap),
    ref_kinetic_energy_(reference_particle.kinetic_energy()),
    kick_voltage_(h_gap*E_hor){

  E_field_base_(0) = E_hor;
  B_field_base_(1) = B_vert;
}

void WFStraight::front_kick(state_type& state){
  for(int i=0; i<state.rows(); i++)
    state(i, 8) -= kick_voltage_*1e-6/ref_kinetic_energy_; // changes dK of state
}

void WFStraight::rear_kick(state_type& state){
 for(int i=0; i<state.rows(); i++)
    state(i, 8) += kick_voltage_*1e-6/ref_kinetic_energy_; // changes dK of state
}

WFCylindrical::WFCylindrical(Particle& reference_particle,
			     double length, double h_gap,
			     double E_hor, double B_vert,
			     std::string name)
  : Element(reference_particle, curv_comp(reference_particle, E_hor, B_vert), length, name),
    ref_kinetic_energy_(reference_particle.kinetic_energy()){

  double R = 1/curve();
  R1_ = R - h_gap;
  R2_ = R*R/R1_;
  plate_voltage_ = - (E_hor * R * log(R2_/R1_)) / 2;

  E_field_base_(0) = E_hor;
  B_field_base_(1) = B_vert;
}

double WFCylindrical::curv_comp(Particle& reference_particle, double E_hor, double B_vert){
  double gamma = reference_particle.gamma();
  double beta = reference_particle.beta();
  double v = CLIGHT * beta;
  double mv2 = gamma * reference_particle.mass0_kg() * v*v;
  double R = - mv2 / (EZERO * (E_hor - v*B_vert));
  return 1/R;
}

double WFCylindrical::kick_voltage(double x){
  double R0 = 1/curve();
  double VlogR2R12 = - E_field_base_(0)*R0;
  return -plate_voltage_ + VlogR2R12 * log((R0+x)/R1_);
}

vectorized_field_type WFCylindrical::EField(state_type state){
  for(int j=0; j<state.rows(); j++)
    E_field_vectorized_(0, j) = E_field_base_(0)/(1 + curve()*state(j, 0)); // E0/(1 + crv*x)

  return tilt_.transform_*E_field_vectorized_;
}

void WFCylindrical::front_kick(state_type& state){
  for(int i=0; i<state.rows(); i++)
    state(i, 8) -= kick_voltage(state(i, 0))*1e-6/ref_kinetic_energy_; // changes dK of state
}

void WFCylindrical::rear_kick(state_type& state){
  for(int i=0; i<state.rows(); i++)
    state(i, 8) += kick_voltage(state(i, 0))*1e-6/ref_kinetic_energy_; // changes dK of state
}
