#include "Core/elements/quadrupole.h"

using namespace std;
using namespace integrator;
using namespace integrator::element;

VectorizedField MQuad::BField(State state){

  // dipole component from shift is -grad*(y0, x0, 0)
  // where x0, y0 are the new center coordiantes of the shifted quadrupole
  double x0 = shift_.x(), y0 = shift_.y();
  
  for (int j=0; j<state.count(); j++){
    B_field_vectorized_(0, j) = grad_*(state(j, 1) - y0); // g*(y - y0)
    B_field_vectorized_(1, j) = grad_*(state(j, 0) - x0); // g*(x - x0)
    B_field_vectorized_(2, j) = 0;
  }

  return B_field_vectorized_;
}



void MQuad::print(){
  Element::print();
  cout << "grad: " << grad_ << endl;
}
