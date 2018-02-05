#include "rf_element.h"


ERF::ERF(double length, Particle_ptr reference_particle, double acc_length,
      double E_field, double phase, int H_number, std::string name)
  : Element(0, (length==0) ? 5e-4 : length, name){
}
