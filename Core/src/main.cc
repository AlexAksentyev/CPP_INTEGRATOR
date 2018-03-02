
#include <fstream>
#include <iostream>
#include "right_hand_side.h"
#include "particle.h"
#include "element.h"
#include "quadrupole.h"
#include "wien_filter.h"
#include "rf_element.h"
#include "element.h"
#include <string>
#include <stdlib.h>
#include <vector>

int main(int argc, char** argv){
  using namespace std;

  double length = atof(argv[1]);
  double G = atof(argv[2]);
  int pid = atoi(argv[3]);
  
  Particle p(1876, 1.14, G);
  //  ERF e(p, 0, length);
  MQuad e(p, length, 8.6);
  //  Observer e(p);

  // creating the state ensemble
  int num_states = 3;
  State state(num_states, VAR_NUM), deriv(num_states, VAR_NUM);
  state.setZero();
  state.col(0) = Eigen::VectorXd::LinSpaced(num_states, -1e-3, 1e-3); // setting x
  
  // output vectors
  vector<State> x;
  vector<double> s;
  DataLog log(x, s);

  size_t num_steps = e.track_through(state, log);


  cout << num_steps << endl;

  log.write_to_file("integrate");

  log.plot(0, pid);  

  return 0;
}
