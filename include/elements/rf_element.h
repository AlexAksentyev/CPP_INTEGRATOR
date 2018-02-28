
#ifndef RF_ELEMENT_H
#define RF_ELEMENT_H

#include "element.h"
#include "particle.h"
#include <math.h>

namespace element{
  struct RFPars { // container for RF parameters
    double length, E_field, phase;
    int H_number;

    RFPars(double length=0, double E_field=15e5, double phase=.5*M_PI, int H_number=50)
      : length(length), E_field(E_field), phase(phase), H_number(H_number) {}
  };


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

    ERF(Particle& reference_particle,
	double acc_length,
	RFPars rf_pars,
	std::string name="RF")
      : ERF(reference_particle, rf_pars.length, acc_length,
	    rf_pars.E_field, rf_pars.phase, rf_pars.H_number,
	    name){}

    bool is_RF(){return true;}
    VectorizedField EField(rhs::State state);
    VectorizedField EField_prime_s(rhs::State state);
  
    void front_kick(rhs::State& state);
    void rear_kick(rhs::State& state);

    friend std::ostream& operator<<(std::ostream& out_stream, const ERF& element){
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

    // implements advance()
    // version 1 logs intermediate states (before rear kick)
    // version 2 leaves logging to Lattice (hence logging after rear kick)
    size_t track_through(rhs::State& ini_states, data_log::DataLog& observer); 
    size_t track_through(rhs::State& ini_states);
  };
}// element namespace

#endif // RF_ELEMENT_H
