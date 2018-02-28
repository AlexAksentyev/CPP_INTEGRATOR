// TODO:

#include "rf_element.h"
#include <iostream>
#include <iomanip>

std::ostream& operator<<(std::ostream& out_stream, const ERF& element){
  using namespace std;
  out_stream << (static_cast<const Element&>(element)) << endl
	     << setw(15) << "amplitude"
	     << setw(15) << "phase"
	     << setw(15) << "frequency" << endl
	     << setw(15) << element.ampl_
	     << setw(15) << element.phase_
	     << setw(15) << element.w_freq_/2/M_PI;
    
  return out_stream;
}

ERF::ERF(Particle& reference_particle,
	 double length, double acc_length,
	 double E_field, double phase, int H_number,
	 std::string name)
  : Element(reference_particle, 0, (length==0) ? 5e-4 : length, name),
    reference_particle_(reference_particle),
    ampl_(E_field), phase_(phase),
    w_freq_(reference_particle_.revolution_freq(acc_length) * H_number * 2 * M_PI),
    wave_number_(w_freq_/CLIGHT/reference_particle_.beta()),
    kick_voltage_(ampl_*length){}

VectorizedField ERF::EField(State state){
  for (int i=0; i<state.rows(); i++)
    E_field_vectorized_(2, i) = ampl_*cos(wave_number_*state(i, 2) + phase_);

  return tilt.transform*E_field_vectorized_;
}

VectorizedField ERF::EField_prime_s(State state){
  for (int i=0; i<state.rows(); i++)
    E_field_prime_s_vectorized_(2, i) = -wave_number_*ampl_*sin(wave_number_*state(i, 2) + phase_);

  return tilt.transform*E_field_prime_s_vectorized_;
}

void ERF::front_kick(State& state){
  for (int i=0; i<state.rows(); i++)
    state(i, 8) -= kick_voltage_*1e-6/reference_particle_.kinetic_energy();
}

void ERF::rear_kick(State& state){
  for (int i=0; i<state.rows(); i++)
    state(i, 8) += kick_voltage_*1e-6/reference_particle_.kinetic_energy();
}

size_t ERF::track_through(State& ini_states, DataLog& observer){// implements advance()
  //  this->vectorize_fields(ini_states); // handled by Lattice
  front_kick(ini_states);
  double angle, beta;
  for(int i=0; i<ini_states.rows(); i++){
    angle = w_freq_*ini_states(i, 3) + phase_;
    ini_states(i, 8) += kick_voltage_*cos(angle)*1e-6/reference_particle_.kinetic_energy(); // update dK
    ini_states(i, 2) += length(); // update s
    beta = reference_particle_.beta(ini_states(i, 8));
    ini_states(i, 3) += length()/beta/CLIGHT; // update t
  }
  observer(ini_states, ini_states(0, 2));
  rear_kick(ini_states);
  return 0;
}

size_t ERF::track_through(State& ini_states){// implements advance()
  //  this->vectorize_fields(ini_states); // handled by Lattice
  front_kick(ini_states);
  double angle, beta;
  for(int i=0; i<ini_states.rows(); i++){
    angle = w_freq_*ini_states(i, 3) + phase_;
    ini_states(i, 8) += kick_voltage_*cos(angle)*1e-6/reference_particle_.kinetic_energy(); // update dK
    ini_states(i, 2) += length(); // update s
    beta = reference_particle_.beta(ini_states(i, 8));
    ini_states(i, 3) += length()/beta/CLIGHT; // update t
  }
  rear_kick(ini_states);
  return 0;
}
