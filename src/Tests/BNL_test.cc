#include <Lattices/BNL.h>

#include <iostream>
#include <vector>
#include <stdlib.h>

#include <boost/tuple/tuple.hpp>

int main(int argc, char** argv) {

  using namespace std;
  using namespace boost;
  using namespace integrator;

  string home_dir = getenv("HOME");
  string root_dir = home_dir + "/REPOS/CPP_INTEGRATOR";
  string config_dir = root_dir+"/config";

  Particle p = Particle::from_config(config_dir + "/particle.conf");
  Lattice lattice = lattice::compose(p, "BNL");

  double tilt_mean = atof(argv[1]);
  double tilt_sig = atof(argv[2]);
  int num_turns = atoi(argv[3]);

  State state = State::from_config(config_dir + "/state.conf");
  data_log::DataLog log;
  pair<size_t, size_t> turn_eid = lattice.track_through(state, log, num_turns);

  cout << "plotting ... \n";
  log.plot("Sz", "Sx", 0, "points");
  log.plot("Sz", "Sy", 0, "points");
  log.plot("Sy", "s", 0, "points");
  // log.plot("x", "s", 0, "linespoints");
  // log.plot("y", "s", 0, "linespoints");
  
  return 0;
}
