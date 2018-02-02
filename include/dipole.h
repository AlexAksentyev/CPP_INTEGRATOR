#ifndef DIPOLE_H
#define DIPOLE_H


#include "element.h"
#include "particle.h"

class MDipole : public Element {


public:
  MDipole(double length, Particle_ptr reference_particle,
	  double By, std::string name="MDip");
};

#endif
