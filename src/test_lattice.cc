#include <fstream>
#include <iostream>
#include "right_hand_side.h"
#include "particle.h"
#include "drift_space.h"
#include "dipole.h"
#include "quadrupole.h"
#include "sextupole.h"
#include "wien_filter.h"
#include "rf_element.h"
#include <string>
#include <stdlib.h>
#include <vector>
#include "lattice.h"

int main(int argc, char** argv) {
  using namespace std;
  using namespace boost;

  int var_id = atoi(argv[1]);
  int pid = atoi(argv[2]);
  double length = atof(argv[3]);
  
   // creating the state ensemble
  int num_states = 3;
  State state(num_states, VAR_NUM), deriv(num_states, VAR_NUM);
  state.setZero();
  state.col(0) = Eigen::VectorXd::LinSpaced(num_states, -1e-3, 1e-3); // setting x

  // output vectors
  vector<State> x;
  vector<double> s;
  DataLog log(x, s);

  // defining the default particle
  Particle p(1876, 1.14, .2);

  // creating a lattice
  Lattice lattice("test");
  lattice = {new MQuad(p, length, 8.6, "QF"),
	     new Drift(p, length/10, "OD"),
	     new MQuad(p, length, -8.4, "QD")};

  cout << lattice.size() << " " << lattice.length() << endl;

  size_t num_steps = 0;
  double current_s = 0;
  for(Lattice::iterator element=lattice.begin();
      element!=lattice.end();
      ++element){
    num_steps += element->track_through(state, log, current_s);
    log.plot(var_id, pid, "lines");
    cout << current_s << endl;
    current_s += element->length();
  }

  log.write_to_file("test_lattice");

  log.plot(var_id, pid, "lines");

  cout << num_steps << endl;
  
  return num_steps;
}
