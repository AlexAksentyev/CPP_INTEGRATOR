
#include "operator_plus.h"
#include <iostream>
#include "elements/element.h"
#include "particle.h"

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
  first = {new Element(p, 0, 1), new Element(p, 0, 2), new Element(p, 0, 3), new Element(p, 0, 4)};
  Lattice second(216); cout << second.length() << endl;
  second = {new Element(p, 0, -1), new Element(p, 0, -2), new Element(p, 0, -3)};
  
  Lattice third(second);
  third = first + second;
  cout << third << endl;

  // cout << third << endl;
  // cout << "length: " << third.length() << endl;;
  
  return 0;
}
