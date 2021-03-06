#ifndef QUADRUPOLE_H
#define QUADRUPOLE_H


#include "element.h"


namespace element{

  struct MQuadPars {
    double length, grad;
    std::string name;

    MQuadPars(double length, double grad, std::string name="MQuad")
      : length(length), grad(grad), name(name) {}
  };
  
  class MQuad : public Element {

    double grad_;
  
  public:
  
    MQuad(Particle& particle,
	  double length, double gradient,
	  std::string name="MQuad")
      : Element(particle, 0, length, name), grad_(gradient) {}
    MQuad(Particle& particle, MQuadPars mqpars)
      : MQuad(particle, mqpars.length, mqpars.grad, mqpars.name) {}
    MQuad(Particle& particle, GradFieldPars gfpars)
      : MQuad(particle, gfpars.length, gfpars.grad, gfpars.name) {}

    VectorizedField BField(rhs::State state_matrix);

    void print();
  };
} // element namespace

#endif // QUADRUPOLE_H
