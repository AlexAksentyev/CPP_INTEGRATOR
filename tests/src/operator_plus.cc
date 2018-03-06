
#include "operator_plus.h"
#include <iostream>

using namespace std;

Lattice& Lattice::operator+=(const Lattice& another){
  this->reserve(this->size() + another.size());
  this->insert(this->end(), another.begin(), another.end());
  length_ += another.length_;
  return (*this);
}


int main () {

  Lattice first(1);
  first = {new double(1), new double(6), new double(7), new double(8)};
  Lattice second(1);
  second = {new double(-4), new double(-5), new double(-2)};

  second += first;

  Lattice third(second);
  cout << third << endl;
  cout << "length: " << third.length() << endl;;
  
  return 0;
}
