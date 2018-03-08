#ifndef DIPOLE_H
#define DIPOLE_H

// Defines classes:
// * magnetic dipole MDipole
// * magnetic solenoid MSolenoid

#include "Core/elements/element.h"
#include "Core/particle.h"

namespace integrator {
  namespace element{
    struct MDipPars { // can be used to initialize all dipole-like elements
      double length, B_field;
      std::string name;

      MDipPars(double length, double B_field, std::string name)
	: length(length), B_field(B_field), name(name) {}
    };

  
    class MDipole : public Element {
      virtual Element* do_clone() const {return new MDipole(*this);}
    public:
      MDipole(Particle& reference_particle,
	      double length, double B_vert,
	      std::string name="MDip");
      MDipole(Particle& reference_particle, MDipPars mdpars)
	: MDipole(reference_particle, mdpars.length, mdpars.B_field, mdpars.name) {}
      MDipole(Particle& reference_particle, UniFieldPars ufpars)
	: MDipole(reference_particle, ufpars.length, ufpars.field, ufpars.name) {}
    };

    class MSolenoid : public Element {
      virtual Element* do_clone() const {return new MSolenoid(*this);}
    public:
      MSolenoid(Particle& particle,
		double length, double B_longitudinal,
		std::string name="MSol");
      MSolenoid(Particle& particle, MDipPars mdpars)
	: MSolenoid(particle, mdpars.length, mdpars.B_field, mdpars.name) {}
      MSolenoid(Particle& particle, UniFieldPars ufpars)
	: MSolenoid(particle, ufpars.length, ufpars.field, ufpars.name) {}
    
    };
  } // element namespace
} // namespace integrator

#endif // DIPOLE_H
