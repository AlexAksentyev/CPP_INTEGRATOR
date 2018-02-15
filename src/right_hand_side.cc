#include "right_hand_side.h"
#include "element.h"
#include <boost/numeric/odeint.hpp>

RightHandSide::RightHandSide(Particle& reference, Element& host)
  : particle_(reference), host_(host) {}


void RightHandSide::operator() (const state_type &state,
				state_type &derivative,
				const double /* s*/){

  int state_num = state.rows();


  double P0c = particle_.Pc();
  variable_col Px = state.col(6)*P0c;
  variable_col Py = state.col(7)*P0c;

  variable_col dK = state.col(8);
  variable_col Pc = particle_.Pc(dK);
  variable_col Ps = sqrt(Pc*Pc - Px*Px - Py*Py);

  variable_col kin_energy = particle_.kinetic_energy(dK);
  
  vectorized_field_type E_field = host_.EField(state);
  vectorized_field_type B_field = host_.BField(state);
  
}

size_t RightHandSide::integrate(state_type ini_states, DataLog& observer){
  using namespace boost::numeric::odeint;
  runge_kutta_dopri5<state_type, double,
		     state_type, double,
		     vector_space_algebra> stepper;
  double delta_s = .1;
  host_.front_kick(ini_states);
  size_t num_steps = integrate_adaptive(stepper, *this, ini_states, 0., host_.length(), delta_s, observer);
  host_.rear_kick(ini_states);

  return num_steps;
}
