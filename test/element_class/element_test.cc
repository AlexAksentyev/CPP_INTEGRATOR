
#include <fstream>
#include <iostream>
#include "right_hand_side.h"
#include "particle.h"
// #include "drift_space.h"
#include "dipole.h"
#include "quadrupole.h"
// #include "sextupole.h"
#include "wien_filter.h"
// #include "rf_element.h"
#include <string>
#include <stdlib.h>
#include <vector>

#include <boost/numeric/odeint.hpp>
#include "boost/numeric/odeint/external/eigen/eigen.hpp"

int main(int argc, char** argv){
  using namespace std;

  double length = atof(argv[1]);
  double G = atof(argv[2]);

  string ROOT_DIR = "/home/alexa/REPOS/CPP_INTEGRATOR/test/element_class";
 
  Particle p(1876, 1.14, G);
  MQuad e(p, length, 8.6);
  RightHandSide rhs(p, e);

  // creating the state ensemble
  int num_states = 2;
  state_type state(num_states, VAR_NUM), deriv(num_states, VAR_NUM);
  state.setZero();
  state.col(0) = Eigen::VectorXd::LinSpaced(num_states, -1e-3, 1e-3); // setting x
  
  // output vectors
  vector<state_type> x;
  vector<double> s;
  DataLog log(x, s);

  e.vectorize_fields(state);
  size_t num_steps = rhs.integrate(state, log);

  cout << num_steps << endl;

  ofstream data_file;
  data_file.open((ROOT_DIR+"/data/integrate.dat").c_str());
  int col_width = 10;
  data_file << fixed << setprecision(4);
  data_file << right
  	    << setw(col_width) << "#time";
  for (int j=0; j<VAR_NUM; j++)
    data_file << setw(col_width) << VAR_NAME[j];
  
  data_file << endl;
  
  for (size_t i=0; i<num_steps; i++){
    data_file << right
	      << setw(col_width) << s[i];

    for (int j=0; j<VAR_NUM; j++)
      data_file << setw(col_width) << x[i](0,j);

    data_file << endl;
  	      
  }

  data_file.close();

  return 0;
}



