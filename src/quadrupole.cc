#include "quadrupole.h"

using namespace std;

void MQuad::print(){
  Element::print();
  cout << "grad: " << grad_ << endl;
}
