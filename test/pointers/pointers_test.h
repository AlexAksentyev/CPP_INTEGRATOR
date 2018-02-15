#include <iostream>

class Particle {
public:
  double mass_;

  Particle(double mass): mass_(mass){}

};

class Element;
class RHS {

  Element& host_;
  Particle& reference_;

public:
  RHS(Element& element, Particle& particle)
    : host_(element), reference_(particle) {}

  double host_length();

  double reference_mass();

};

class Element {
public:
  RHS rhs_;
  double length_;
  
  Element(double length, Particle& reference)
    : length_(length), rhs_((*this), reference) {}

  void operator()();

};
