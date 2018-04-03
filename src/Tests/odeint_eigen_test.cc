#include <Core/particle.h>
#include <Core/right_hand_side.h>
#include <Core/elements/quadrupole.h>

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <Eigen/Dense>

int main(int argc, char** argv){

  using namespace integrator;
  using namespace integrator::rhs;
  using namespace integrator::element;

  using namespace Eigen;
  using namespace std;

  Particle p;
  MQuad qf(p, 25e-2, 8.6);
  RightHandSide rhs(p, qf);

  size_t num_state = 2000;

  State x0(num_state, VAR_NUM), der(num_state, VAR_NUM);
  x0 = State::Zero(num_state, VAR_NUM);
  x0.col(0) = ArrayXd::LinSpaced(num_state, -1e-3,  1e-3);
  x0.col(1) = ArrayXd::LinSpaced(num_state,  1e-3, -1e-3);

  qf.vectorize_fields(x0);

  int reps = atoi(argv[1]);
  
  clock_t t;
  t = clock();
  for (int i=0; i<reps; i++){
    rhs(x0, x0, 0);
  }
  cout << "Run time: " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
  

}
