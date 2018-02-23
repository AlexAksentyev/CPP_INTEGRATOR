// TODO :
// * Finish the RF element


#ifndef RF_ELEMENT_H
#define RF_ELEMENT_H

#include "element.h"
#include "particle.h"
#include <math.h>


class ERF : public Element {
  Particle& reference_particle_;
  double ampl_, phase_, w_freq_;
  double wave_number_;
  double kick_voltage_;
public:
  ERF(Particle& reference_particle,
      double length, double acc_length,
      double E_field=15e5, double phase=0.5*M_PI, int H_number=50,
      std::string name="RF");

  VectorizedField EField(State state);
  VectorizedField EField_prime_s(State state);
  
  void front_kick(State& state);
  void rear_kick(State& state);

  size_t track_through(State& ini_states, DataLog& observer); // implements advance()
};


#endif
