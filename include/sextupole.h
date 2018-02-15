#ifndef SEXTUPOLE_H
#define SEXTUPOLE_H

#include "element.h"

class MSext : public Element {
  double grad_;

public:
  MSext(Particle& particle, double length, double gradient, std::string name="MSext")
    : Element(particle, 0, length, name), grad_(gradient){}

  vectorized_field_type BField(state_type state);
};


#endif
