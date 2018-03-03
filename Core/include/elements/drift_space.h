#ifndef DRIFT_SPACE_H
#define DRIFT_SPACE_H


#include "element.h"

namespace integrator {
  namespace element{
    class Drift : public Element {
    public:
      Drift(Particle& particle, double length, std::string name="Drift")
	: Element(particle, 0, length, name) {}
    };
  } // element namespace
} // namespace integrator

#endif // DRIFT_SPACE_H
