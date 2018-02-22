#include "quadrupole.h"

using namespace std;
using namespace Eigen;

vectorized_field_type MQuad::BField(state_type state){
  for (int j=0; j<state.rows(); j++){
      B_field_vectorized_(0, j) = grad_*state(j, 1); // g*y
      B_field_vectorized_(1, j) = grad_*state(j, 0); // g*x
      B_field_vectorized_(2, j) = 0;
    }

  return tilt_.transform_*B_field_vectorized_;
}



void MQuad::print(){
  Element::print();
  cout << "grad: " << grad_ << endl;
}