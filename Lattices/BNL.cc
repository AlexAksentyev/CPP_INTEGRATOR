#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include <right_hand_side.h>
#include "particle.h"
#include <lattice.h>

#include <elements/drift_space.h>
#include <elements/dipole.h>
#include <elements/quadrupole.h>
#include <elements/sextupole.h>
#include <elements/wien_filter.h>
#include <elements/rf_element.h>

#include <boost/tuple/tuple.hpp>

#include <iomanip>
#include <time.h>

#include "read_matrix.h" // read State information from .conf file

using namespace integrator;

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
  
   // creating the state ensemble
  string home_dir = getenv("HOME");
  string config_dir = home_dir+"/REPOS/CPP_INTEGRATOR/Lattices/config";
  cout << "Reading state config" << endl;
  rhs::State state = utilities::read_matrix<rhs::State>(config_dir + "/state.conf");

  // defining the particle
  cout << "Reading particle config" << endl;
  Particle p = Particle::from_config(config_dir + "/particle.conf"); // (1876, 1.14, .2);
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

  cout << "\t Making lattice segments" << endl;
  
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

  cout << " \t Made SS1H2" << endl;
  // Lattice ARC1("ARC1");
  // ARC1 = {new MQuad(p, qfa1_pars),
  // 	  new Drift(p, od_len, "OD1"),
  // 	  new MSext(p, sfp_pars),
  // 	  new Drift(p, od_len, "OD2"),
  // 	  new WFCylindrical(p, rbe_pars),
  // 	  new Drift(p, od_len, "OD2"),
  // 	  new Drift(p, bpm_len, "BPM"),
  // 	  new Drift(p, od_len, "OD1"),
  // 	  new MQuad(p, qda1_pars),
  // 	  new MQuad(p, qda1_pars),
  // 	  new Drift(p, od_len, "OD1"),
  // 	  new MSext(p, sdp_pars),
  // 	  new Drift(p, od_len, "OD2"),
  // 	  new WFCylindrical(p, rbe_pars),
  // 	  new Drift(p, od_len, "OD2"),
  // 	  new Drift(p, bpm_len, "BPM"),
  // 	  new Drift(p, od_len, "OD1"),
  // 	  new MQuad(p, qfa1_pars)};
  // ARC1 = ARC1.replicate(8);
  // cout << "\t Made ARC1" << endl;
  // Lattice SS2H1("SS2H1");
  // SS2H1 = {new MQuad(p, qfa2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MSext(p, sfp_pars),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qda2_pars),
  // 	   //
  // 	   new MQuad(p, qda2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MSext(p, sdp_pars),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qfa2_pars),
  // 	   //
  // 	   new MQuad(p, qfa2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MSext(p, sfp_pars),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qda2_pars)};
  // Lattice SS2H2("SS2H2");
  // SS2H2 = {new MQuad(p, qda2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new Drift(p, os_len, "OSD"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qfa2_pars),
  // 	   //
  // 	   new MQuad(p, qfa2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new Drift(p, os_len, "OSF"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qda2_pars),
  // 	   //
  // 	   new MQuad(p, qda2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new Drift(p, os_len, "OSD"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qfa2_pars)};
  // cout << "\t Made SS2" << endl;
  // Lattice ARC2("ARC2");
  // ARC2 = {new MQuad(p, qfa1_pars),
  // 	  new Drift(p, od_len, "OD1"),
  // 	  new MSext(p, sfp_pars),
  // 	  new Drift(p, od_len, "OD2"),
  // 	  new WFCylindrical(p, rbe_pars),
  // 	  new Drift(p, od_len, "OD2"),
  // 	  new Drift(p, bpm_len, "BPM"),
  // 	  new Drift(p, od_len, "OD1"),
  // 	  new MQuad(p, qda1_pars),
  // 	  new MQuad(p, qda1_pars),
  // 	  new Drift(p, od_len, "OD1"),
  // 	  new MSext(p, sdp_pars),
  // 	  new Drift(p, od_len, "OD2"),
  // 	  new WFCylindrical(p, rbe_pars),
  // 	  new Drift(p, od_len, "OD2"),
  // 	  new Drift(p, bpm_len, "BPM"),
  // 	  new Drift(p, od_len, "OD1"),
  // 	  new MQuad(p, qfa1_pars)};
  // ARC2 = ARC2.replicate(8);
  // cout << "\t Made ARC2" << endl;
  // Lattice SS1H1("SS1H1");
  // SS1H1 = {new MQuad(p, qfa2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MSext(p, sfp_pars),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qda2_pars),
  // 	   //
  // 	   new MQuad(p, qda2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MSext(p, sdp_pars),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qfa2_pars),
  // 	   //
  // 	   new MQuad(p, qfa2_pars),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MSext(p, sfp_pars),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, orb_len, "ORB"),
  // 	   new Drift(p, od_len, "OD2"),
  // 	   new Drift(p, bpm_len, "BPM"),
  // 	   new Drift(p, od_len, "OD1"),
  // 	   new MQuad(p, qda2_pars)};

  cout << "\t Made all" << endl;

  Lattice lattice("BNL");
  cout << "\t Combining segments" << endl;
   SS1H2 += SS1H2;
   cout << "\t added" << endl;

  RFPars rf_pars;
  rf_pars.E_field=15e7;
  //  lattice.insert_RF(0, p, rf_pars);

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
  out_file.open("../data/BNL.dat");
  out_file << scientific << setprecision(4) << log;
  out_file.close();

  cout << "plotting ... \n";
  log.plot("x", "s", 0, "points");
  
  return 0;
}
