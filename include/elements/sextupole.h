#ifndef SEXTUPOLE_H
#define SEXTUPOLE_H

#include "element.h"

namespace element{
  class MSext : public Element {
    double grad_;

  public:
    MSext(Particle& particle, double length, double gradient, std::string name="MSext")
      : Element(particle, 0, length, name), grad_(gradient){}

    VectorizedField BField(RHS::State state);
  };
} // element namespace

#endif // SEXTUPOLE_H
