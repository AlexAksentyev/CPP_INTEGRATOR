#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include <Core/right_hand_side.h>
#include "Core/particle.h"
#include <Core/lattice.h>

#include <Core/elements/drift_space.h>
#include <Core/elements/dipole.h>
#include <Core/elements/quadrupole.h>
#include <Core/elements/sextupole.h>
#include <Core/elements/wien_filter.h>
#include <Core/elements/rf_element.h>

#include <boost/tuple/tuple.hpp>

#include <iomanip>
#include <time.h>

#include "Utilities/read_matrix.h" // read State information from .conf file

using namespace integrator;

void print_lattice_elements(Lattice& lattice){
  using namespace std;
  // print lattice element names
  for (Lattice::element_iterator element=lattice.begin();
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
  
   // creating the state ensemble
  string home_dir = getenv("HOME");
  string config_dir = home_dir+"/REPOS/CPP_INTEGRATOR/Lattices/config";
  //  string filename = config_dir + "/state.conf";
  rhs::State state = utilities::read_matrix<rhs::State>(config_dir + "/state.conf");

  // defining the default particle
  cout << "Reading particle config" << endl;
  // filename = config_dir + "/particle.conf";
  Particle p = Particle::from_config(config_dir + "/particle.conf"); // (1876, 1.14, .2);
  p.print();
  
  // creating a lattice
  GradFieldPars qda2_pars(5e-2, -8.2, "QDA2");
  GradFieldPars qfa2_pars(5e-2, 7.36, "QFA2");
  double od_len = 25e-2, orb_len=2.2, os_len=15e-2, bpm_len=15e-2;
  
  Lattice lattice("FODO");
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
  clock_t t;
  t=clock();
  pair<size_t, size_t> turn_eid = lattice.track_through(state, log, num_turns);
  t = clock() - t;
  printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);

  cout << "exited at:\n"
       << "\t turn: " << turn_eid.first << endl
       << "\t eid: " << turn_eid.second << endl;

  cout << log.size() << endl;
  ofstream out_file;
  out_file.open("../data/FODO_lattice.dat");
  out_file << scientific << setprecision(4) << log;
  out_file.close();

  cout << "plotting ... \n";
  log.plot("x", "s", 1, "points");
  
  return 0;
}
