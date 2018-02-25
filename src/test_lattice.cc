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

#include<boost/tuple/tuple.hpp>

int main(int argc, char** argv) {
  using namespace std;
  using namespace boost;

  using TiltTuple = boost::tuple<char, double, double>;
  vector<TiltTuple> axis_mean_sigma;

  char axis; double mean, sigma;
  for(int i=1; i<argc-2; i+=3){
    axis = *argv[i];
    mean = atof(argv[i+1]);
    sigma = atof(argv[i+2]);
    axis_mean_sigma.push_back(TiltTuple(axis, mean, sigma));
  }
  

  int var_id = 9;
  int pid = 0;
  double length = 1;
  
   // creating the state ensemble
  int num_states = 3;
  State state(num_states, VAR_NUM), deriv(num_states, VAR_NUM);
  state.setZero();
  state.col(0) = Eigen::VectorXd::LinSpaced(num_states, -1e-3, 1e-3); // setting x
  state.col(11) = Eigen::VectorXd::Ones(num_states); // Sz = 1

  // output vectors
  vector<State> x;
  vector<double> s;
  DataLog log(x, s);

  // defining the default particle
  Particle p(1876, 1.14, .2);

  // creating a lattice
  Lattice lattice("test");
  lattice = {new MDipole(p, length, 1.5, "BDA")};//,
	     // new MQuad(p, length, 8.6, "QF"),
	     // new Drift(p, length/10, "OD"),
	     // new MQuad(p, length, -8.4, "QD")};

  lattice.tilt(axis_mean_sigma);
  lattice[0].tilt_.print();

  MDipole* e = (MDipole*)&lattice[0];
  cout << "rotation map: \n" << e->tilt_.transform_.rotation() << endl;
  e->vectorize_fields(state);
  cout << "B-field: \n" << e->BField(state) << endl;
  
  size_t num_steps = 0;
  double current_s = 0;
  for(Lattice::iterator element=lattice.begin();
      element!=lattice.end();
      ++element){
    num_steps += element->track_through(state, log, current_s);
    // log.plot(var_id, pid, "lines");
    current_s += element->length();
  }

  log.write_to_file("test_lattice");

  log.plot(9, pid, "lines");
  log.plot(10, pid, "lines");

  cout << "integration steps: " << num_steps << endl;
  
  return num_steps;
}
