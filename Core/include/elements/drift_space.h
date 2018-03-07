#ifndef DRIFT_SPACE_H
#define DRIFT_SPACE_H


#include "elements/element.h"

namespace integrator {
  namespace element{
    class Drift : public Element {
      virtual Element* do_clone() const {return new Drift(*this);}
    public:
      Drift(Particle& particle, double length, std::string name="Drift")
	: Element(particle, 0, length, name) {}
    };
  } // element namespace
} // namespace integrator

#endif // DRIFT_SPACE_H
