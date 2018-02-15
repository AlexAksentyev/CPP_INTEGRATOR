// TODO :
// * Finish the RF element


#ifndef RF_ELEMENT_H
#define RF_ELEMENT_H

#include "element.h"
#include "particle.h"
#include <math.h>


class ERF : public Element {

public:
  ERF(Particle& reference_particle,
      double length, double acc_length,
      double E_field=15e5, double phase=1.5*M_PI, int H_number=50,
      std::string name="RF");

};


#endif
