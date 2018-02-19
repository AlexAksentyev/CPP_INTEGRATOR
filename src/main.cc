
#include <fstream>
#include <iostream>
#include "right_hand_side.h"
#include "particle.h"
#include "quadrupole.h"
#include "dipole.h"
#include "sextupole.h"
#include "wien_filter.h"
#include "rf_element.h"
#include <string>
#include <stdlib.h>
#include <vector>

// #include "gnuplot-iostream.h"
// #include <boost/tuple/tuple.hpp>

// #include <boost/numeric/odeint.hpp>
// #include "boost/numeric/odeint/external/eigen/eigen.hpp"


std::string to_string(double x);

int main(int argc, char** argv){
  using namespace std;

  double length = atof(argv[1]);
  double G = atof(argv[2]);
  int pid = atoi(argv[3]);

  string ROOT_DIR = "/home/alexa/REPOS/CPP_INTEGRATOR/test/element_class";
 
  Particle p(1876, 1.14, G);
  MQuad e(p, length, 8.24);
  RightHandSide rhs(p, e);

  // creating the state ensemble
  int num_states = 3;
  state_type state(num_states, VAR_NUM), deriv(num_states, VAR_NUM);
  state.setZero();
  state.col(0) = Eigen::VectorXd::LinSpaced(num_states, -1e-3, 1e-3); // setting x
  
  // output vectors
  vector<state_type> x;
  vector<double> s;
  DataLog log(x, s);

  e.vectorize_fields(state);
  size_t num_steps = e.track_through(state, log);

  cout << num_steps << endl;

  log.write_to_file("integrate", ROOT_DIR+"/data");

  log.plot(0, pid);

  return 0;
}
