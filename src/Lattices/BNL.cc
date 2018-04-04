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
  string root_dir = home_dir + "/REPOS/CPP_INTEGRATOR";
  string config_dir = root_dir+"/config";
  cout << "Reading state config" << endl;
  rhs::State state = utilities::read_matrix<rhs::State>(config_dir + "/state.conf");

  // defining the particle
  cout << "Reading particle config" << endl;
  Particle p = Particle::from_config(config_dir + "/particle.conf");
  p.print();
  
  // creating a lattice
  GradFieldPars qda1_pars(5e-2, -10.23, "QDA1"), qda2_pars(5e-2, -8.6, "QDA2");
  GradFieldPars qfa1_pars(5e-2,  13.64, "QFA1"), qfa2_pars(5e-2, 8.31, "QFA2");
  GradFieldPars sdp_pars(15e-2, -3.39597809*0, "SDP"), sfp_pars(15e-2,  2.7695769*0, "SFP");
  GradFieldPars sdn_pars(15e-2,  3.79310524*0,"SDN"), sfn_pars(15e-2, -2.09836542*0,"SFN");
  double od_len = 25e-2,
    orb_len=2.2, ore_len=2.17,
    os_len=15e-2, bpm_len=15e-2;
  WFPars rbe_pars(180.77969e-2, 5e-2, 120e5, 0.46002779, "RBE");
  
  Lattice SS1H2("SS1H2");
  SS1H2 = {new MQuad(p, qda2_pars),
	   new Drift(p, od_len, "OD1"),
	   new Drift(p, os_len, "OSD"),
	   new Drift(p, od_len, "OD2"),
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
  cout << "SS1H2 size: "<< SS1H2.element_count() << endl;
  Lattice ARC1("ARC1");
  ARC1 = {new MQuad(p, qfa1_pars),
  	  new Drift(p, od_len, "OD1"),
  	  new MSext(p, sfp_pars),
  	  new Drift(p, od_len, "OD2"),
  	  new WFCylindrical(p, rbe_pars),
  	  new Drift(p, od_len, "OD2"),
  	  new Drift(p, bpm_len, "BPM"),
  	  new Drift(p, od_len, "OD1"),
  	  new MQuad(p, qda1_pars),
	  //
  	  new MQuad(p, qda1_pars),
  	  new Drift(p, od_len, "OD1"),
  	  new MSext(p, sdp_pars),
  	  new Drift(p, od_len, "OD2"),
  	  new WFCylindrical(p, rbe_pars),
  	  new Drift(p, od_len, "OD2"),
  	  new Drift(p, bpm_len, "BPM"),
  	  new Drift(p, od_len, "OD1"),
  	  new MQuad(p, qfa1_pars)};
  ARC1 = ARC1.replicate(8);
  cout << "ARC1 size: " << ARC1.element_count() << endl;
  Lattice SS2H1("SS2H1");
  SS2H1 = {new MQuad(p, qfa2_pars),
  	   new Drift(p, od_len, "OD1"),
  	   new MSext(p, sfp_pars),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, orb_len, "ORB"),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, bpm_len, "BPM"),
  	   new Drift(p, od_len, "OD1"),
  	   new MQuad(p, qda2_pars),
  	   //
  	   new MQuad(p, qda2_pars),
  	   new Drift(p, od_len, "OD1"),
  	   new MSext(p, sdp_pars),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, orb_len, "ORB"),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, bpm_len, "BPM"),
  	   new Drift(p, od_len, "OD1"),
  	   new MQuad(p, qfa2_pars),
  	   //
  	   new MQuad(p, qfa2_pars),
  	   new Drift(p, od_len, "OD1"),
  	   new MSext(p, sfp_pars),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, orb_len, "ORB"),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, bpm_len, "BPM"),
  	   new Drift(p, od_len, "OD1"),
  	   new MQuad(p, qda2_pars)};
  cout << "SS2H1 size: "<< SS2H1.element_count() << endl;
  Lattice SS2H2("SS2H2");
  SS2H2 = {new MQuad(p, qda2_pars),
  	   new Drift(p, od_len, "OD1"),
  	   new Drift(p, os_len, "OSD"),
  	   new Drift(p, od_len, "OD2"),
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
  cout << "SS2H2 size: "<< SS2H2.element_count() << endl;
  Lattice ARC2(ARC1);
  ARC2.rename("ARC2");
  cout << "ARC2 size: " << ARC2.element_count() << endl;
  Lattice SS1H1("SS1H1");
  SS1H1 = {new MQuad(p, qfa2_pars),
  	   new Drift(p, od_len, "OD1"),
  	   new MSext(p, sfp_pars),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, orb_len, "ORB"),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, bpm_len, "BPM"),
  	   new Drift(p, od_len, "OD1"),
  	   new MQuad(p, qda2_pars),
  	   //
  	   new MQuad(p, qda2_pars),
  	   new Drift(p, od_len, "OD1"),
  	   new MSext(p, sdp_pars),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, orb_len, "ORB"),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, bpm_len, "BPM"),
  	   new Drift(p, od_len, "OD1"),
  	   new MQuad(p, qfa2_pars),
  	   //
  	   new MQuad(p, qfa2_pars),
  	   new Drift(p, od_len, "OD1"),
  	   new MSext(p, sfp_pars),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, orb_len, "ORB"),
  	   new Drift(p, od_len, "OD2"),
  	   new Drift(p, bpm_len, "BPM"),
  	   new Drift(p, od_len, "OD1"),
  	   new MQuad(p, qda2_pars)};

  cout << "SS1H1 size: " << SS1H1.element_count() << endl;
  Lattice lattice("BNL");
  cout << "BNL lattice element count: "<< lattice.element_count() << endl;
  lattice << SS1H2;
  cout << "BNL lattice element count: "<< lattice.element_count() << endl;
  lattice << ARC1;
  cout << "BNL lattice element count: "<< lattice.element_count() << endl;
  lattice << SS2H1;
  cout << "BNL lattice element count: "<< lattice.element_count() << endl;
  lattice << SS2H2;
  cout << "BNL lattice element count: "<< lattice.element_count() << endl;
  lattice << ARC2;
  cout << "BNL lattice element count: "<< lattice.element_count() << endl;
  lattice << SS1H1;
  
  RFPars rf_pars;
  rf_pars.E_field=15e7;
  lattice.insert_RF(0, p, rf_pars);
  cout << "BNL lattice element count: "<< lattice.element_count() << endl;
  cout << "BNL lattice length: "<< lattice.length() << endl;

  vector<boost::tuple<char, double, double>> tilts;
  boost::tuple<char, double, double> tilt1('s', .0057, 0);
  tilts.push_back(tilt1);
  lattice.tilt(tilts);
  
  data_log::DataLog log;
  clock_t t;
  cout << "Starting tracking" << endl;
  t=clock();
  pair<size_t, size_t> turn_eid = lattice.track_through(state, log, num_turns);
  t = clock() - t;
  float secs = ((float)t)/CLOCKS_PER_SEC;
  printf ("It took me %d clicks (%f seconds).\n", t, secs);
  printf("Or, %f secs/turn.\n", secs/num_turns);

  cout << "exited at:\n"
       << "\t turn: " << turn_eid.first << endl
       << "\t eid: " << turn_eid.second << endl;

  cout << "Log #records: " << log.size() << endl;
  ofstream out_file;
  out_file.open(root_dir + "/data/BNL.dat");
  out_file << scientific << setprecision(4) << log;
  out_file.close();

  cout << "plotting ... \n";
  log.plot("Sx", "s", 0, "points");
  log.plot("Sy", "s", 0, "points");
  
  return 0;
}
