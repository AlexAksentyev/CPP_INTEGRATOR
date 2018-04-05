#ifndef SEXTUPOLE_H
#define SEXTUPOLE_H

#include "Core/elements/element.h"

namespace integrator {
  namespace core {
    namespace element{
      struct MSextPars {
	double length, grad;
	std::string name;

	MSextPars(double length, double grad, std::string name="MSext")
	  : length(length), grad(grad), name(name) {}
      };
  
      class MSext : public Element {
	double grad_;
	virtual Element* do_clone() const {return new MSext(*this);}
      public:
	MSext(Particle& particle, double length, double gradient, std::string name="MSext")
	  : Element(particle, 0, length, name), grad_(gradient){}
	MSext(Particle& particle, MSextPars mspars)
	  : MSext(particle, mspars.length, mspars.grad, mspars.name) {}
	MSext(Particle& particle, GradFieldPars gfpars)
	  : MSext(particle, gfpars.length, gfpars.grad, gfpars.name) {}

	VectorizedField BField(State state);
      };
    } // element namespace
  } // namespace core
} //namespace integrator
#endif // SEXTUPOLE_H
