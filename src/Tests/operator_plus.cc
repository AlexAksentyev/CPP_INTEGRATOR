
#include "Tests/operator_plus.h"
#include <iostream>
#include "Core/elements/quadrupole.h"
#include "Core/particle.h"

using namespace std;
using namespace integrator;
using namespace integrator::element;

Lattice& Lattice::operator+=(Lattice& another){
  seq_.reserve(seq_.size() + another.size());
  seq_.insert(seq_.end(), another.begin(), another.end());
  length_ += another.length_;
  return (*this);
}

int main () {

  Particle p;

  Lattice first(354); cout << first.length() << endl;
  first = {new MQuad(p, 1, 0), new MQuad(p, 2, 0), new MQuad(p, 3, 0), new MQuad(p, 4, 0)};
  Lattice second(216); cout << second.length() << endl;
  second = {new MQuad(p, -1, 0), new MQuad(p, -2, 0), new MQuad(p, -3, 0)};

  cout  << "first lattice: \n" <<  first << endl;
  
  Lattice third(second);
  cout << "third lattice: \n" << third << endl;
  third += first;
  cout << "third + first latice: \n" << third << endl;

  // cout << third << endl;
  // cout << "length: " << third.length() << endl;;
  
  return 0;
}
