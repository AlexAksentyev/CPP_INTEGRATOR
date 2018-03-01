#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include "right_hand_side.h"
#include "particle.h"
#include "drift_space.h"
#include "dipole.h"
#include "quadrupole.h"
#include "sextupole.h"
#include "wien_filter.h"
#include "rf_element.h"
#include "lattice.h"

#include <boost/tuple/tuple.hpp>

#include <iomanip>

void print_lattice_elements(Lattice& lattice){
  using namespace std;
  // print lattice element names
  for (Lattice::iterator element=lattice.begin();
       element!=lattice.end();
       ++element)
    cout << element->name() << endl;
}

int main(int argc, char** argv) {
  using namespace std;
  using namespace boost;
  using namespace element;

  // parse input arguments
  size_t num_turns = atoi(argv[1]);
  string var_y(argv[2]);
  string var_x(argv[3]);

  int var_id = 9;
  int pid = 1;
  double length = 1;
  
   // creating the state ensemble
  int num_states = 3;
  rhs::State state(num_states, rhs::VAR_NUM), deriv(num_states, rhs::VAR_NUM);
  state.setZero();
  state.col(0) = Eigen::VectorXd::LinSpaced(num_states, -1e-3, 1e-3); // setting x
  state.col(rhs::VMAP.left.at("dK")) = Eigen::VectorXd::LinSpaced(3, 0, 1e-4);
  state.col(11) = Eigen::VectorXd::Ones(num_states); // Sz = 1

  // defining the default particle
  Particle p(1876, 1.14, .2);

  // creating a lattice
  Lattice lattice("test");
  lattice = {new MDipole(p, length, 1.5, "BDA"),
	     new MQuad(p, length, 8.6, "QF"),
	     new Drift(p, length/10, "OD"),
	     new MQuad(p, length, -8.4, "QD")};

  RFPars rf_pars;
  rf_pars.E_field=15e7;
  
  lattice.insert_RF(0, p, rf_pars);

  data_log::DataLog log;

  size_t num_steps = lattice.track_through(state, log, num_turns);

  log.write_to_file("test_lattice");

  log.plot(var_y, var_x, pid, "lines");

  cout << "integration steps: " << num_steps << endl;
  
  return num_steps;
}
