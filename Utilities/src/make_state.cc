
#include "right_hand_side.h"

#include <iostream>
#include <Eigen/Dense>
#include <stdlib.h>
#include <string>

using namespace integrator::rhs;
using namespace std;
using namespace Eigen;

int main(int argc, char** argv){

  int num_state=atoi(argv[1]);
  string var_name = argv[2];
  double lb = atof(argv[3]);
  double ub = atof(argv[4]);
  State state(num_state, VAR_NUM);
  state.setZero();
  state.col(VMAP.left.at(var_name)) = VectorXd::LinSpaced(num_state, lb, ub);
  cout << state;

  return 0;
}
