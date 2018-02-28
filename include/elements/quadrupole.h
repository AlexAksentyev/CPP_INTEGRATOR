#ifndef QUADRUPOLE_H
#define QUADRUPOLE_H


#include "element.h"


namespace element{
  class MQuad : public Element {

    double grad_;
  
  public:
  
    MQuad(Particle& particle,
	  double length, double gradient,
	  std::string name="MQuad")
      : Element(particle, 0, length, name), grad_(gradient) {}

    VectorizedField BField(rhs::State state_matrix);

    void print();
  };
} // element namespace

#endif // QUADRUPOLE_H
