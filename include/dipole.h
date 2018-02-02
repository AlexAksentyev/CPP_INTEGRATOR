#ifndef DIPOLE_H
#define DIPOLE_H

// Defines classes:
// * magnetic dipole MDipole
// * magnetic solenoid MSolenoid

#include "element.h"
#include "particle.h"

class MDipole : public Element {
public:
  MDipole(double length, Particle_ptr reference_particle,
	  double B_vert, std::string name="MDip");
};

class MSolenoid : public Element {
public:
  MSolenoid(double length, double B_longitudinal, std::string name="MSol");
};

#endif
