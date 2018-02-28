#ifndef DIPOLE_H
#define DIPOLE_H

// Defines classes:
// * magnetic dipole MDipole
// * magnetic solenoid MSolenoid

#include "element.h"
#include "particle.h"


namespace element{
  class MDipole : public Element {
  public:
    MDipole(Particle& reference_particle,
	    double length, double B_vert,
	    std::string name="MDip");
  };

  class MSolenoid : public Element {
  public:
    MSolenoid(Particle& particle,
	      double length, double B_longitudinal,
	      std::string name="MSol");
  };
} // element namespace

#endif // DIPOLE_H
