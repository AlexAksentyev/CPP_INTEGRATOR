
#include <Core/particle.h>
#include <Core/right_hand_side.h>
#include <Core/elements/quadrupole.h>
#include <Core/state.h>

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <Eigen/Dense>

int main(int argc, char** argv){

  using namespace core;
  using namespace core::rhs;
  using namespace core::element;

  using namespace Eigen;
  using namespace std;

  Particle p;
  MQuad qf(p, 25e-2, 8.6);
  RightHandSide rhs(p, qf);

  size_t num_state = 2000;

  State::OldState x0(num_state, 12), der(num_state, 12);
  x0 = State::OldState::Zero(num_state, 12);
  x0.col(0) = ArrayXd::LinSpaced(num_state, -1e-3,  1e-3);
  x0.col(1) = ArrayXd::LinSpaced(num_state,  1e-3, -1e-3);

  qf.vectorize_fields(x0);

  State x0_(x0), der_(der);
  int reps = atoi(argv[1]);
  
  clock_t t;
  t = clock();
  for (int i=0; i<reps; i++){

    rhs(x0_, der_, 0);
  }
  cout << "Run time: " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
  

}
