#include "rf_element.h"


ERF::ERF(Particle& reference_particle,
	 double length, double acc_length,
	 double E_field, double phase, int H_number,
	 std::string name)
  : Element(reference_particle, 0, (length==0) ? 5e-4 : length, name){
}
