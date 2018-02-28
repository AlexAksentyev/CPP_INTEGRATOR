#include "quadrupole.h"

using namespace std;
using namespace Eigen;
using namespace RHS;

VectorizedField MQuad::BField(State state){
  for (int j=0; j<state.rows(); j++){
      B_field_vectorized_(0, j) = grad_*state(j, 1); // g*y
      B_field_vectorized_(1, j) = grad_*state(j, 0); // g*x
      B_field_vectorized_(2, j) = 0;
    }

  return tilt.transform*B_field_vectorized_;
}



void MQuad::print(){
  Element::print();
  cout << "grad: " << grad_ << endl;
}
