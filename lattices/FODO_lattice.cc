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
  int pid = atoi(argv[2]);
  string var_y(argv[3]);
  string var_x(argv[4]);
  
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
  GradFieldPars qda2_pars(5e-2, -8.2, "QDA2");
  GradFieldPars qfa2_pars(5e-2, 7.36, "QFA2");
  double od_len = 25e-2, orb_len=2.2, os_len=15e-2, bpm_len=15e-2;
  
  Lattice lattice("test");
  lattice = {new MQuad(p, qda2_pars),
	     new Drift(p, od_len, "OD1"),
	     new Drift(p, orb_len, "ORB"),
	     new Drift(p, od_len, "OD2"),
	     new Drift(p, bpm_len, "BPM"),
	     new Drift(p, od_len, "OD1"),
	     new MQuad(p, qfa2_pars),
	     //
	     new MQuad(p, qfa2_pars),
	     new Drift(p, od_len, "OD1"),
	     new Drift(p, os_len, "OSF"),
	     new Drift(p, od_len, "OD2"),
	     new Drift(p, orb_len, "ORB"),
	     new Drift(p, od_len, "OD2"),
	     new Drift(p, bpm_len, "BPM"),
	     new Drift(p, od_len, "OD1"),
	     new MQuad(p, qda2_pars),
	     //
	     new MQuad(p, qda2_pars),
	     new Drift(p, od_len, "OD1"),
	     new Drift(p, os_len, "OSD"),
	     new Drift(p, od_len, "OD2"),
	     new Drift(p, orb_len, "ORB"),
	     new Drift(p, od_len, "OD2"),
	     new Drift(p, bpm_len, "BPM"),
	     new Drift(p, od_len, "OD1"),
	     new MQuad(p, qfa2_pars)};

  RFPars rf_pars;
  rf_pars.E_field=15e7;
  
  lattice.insert_RF(0, p, rf_pars);

  data_log::DataLog log;

  pair<size_t, size_t> turn_eid = lattice.track_through(state, log, num_turns);

  cout << "exited at:\n"
       << "\t turn: " << turn_eid.first << endl
       << "\t eid: " << turn_eid.second << endl;

  cout << log.size() << endl;
  ofstream out_file;
  out_file.open("../data/FODO_lattice.dat");
  out_file << scientific << setprecision(3) << log;
  out_file.close();

  cout << "plotting ... \n";
  log.plot(var_y, var_x, pid, argv[5]);
  
  return 0;
}
