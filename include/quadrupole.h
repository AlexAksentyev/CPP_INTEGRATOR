#ifndef QUADRUPOLE_H
#define QUADRUPOLE_H


#include "element.h"



class MQuad : public Element {

  double grad_;
  
public:
  
 MQuad(Particle& particle,
       double length, double gradient,
       std::string name="MQuad")
   : Element(particle, 0, length, name), grad_(gradient) {}

  vectorized_field_type BField(state_type state_matrix);

  void print();
};

#endif
