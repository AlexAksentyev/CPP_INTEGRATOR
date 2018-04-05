#include "Core/elements/sextupole.h"

using namespace integrator::core;
using namespace element;

VectorizedField MSext::BField(State state){
  for(int j=0; j<state.count(); j++){
    double x = state(j, 0), y = state(j, 1);
    B_field_vectorized_(0, j) = grad_*x*y;
    B_field_vectorized_(1, j) = .5*grad_*(x*x - y*y);
    B_field_vectorized_(2, j) = 0;
  }
  return B_field_vectorized_;
}
