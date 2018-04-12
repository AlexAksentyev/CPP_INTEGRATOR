
#include <Core/lattice.h>
#include <Core/particle.h>

#include <Core/elements/dipole.h>
#include <Core/elements/wien_filter.h>

#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <Eigen/Dense>


integrator::core::Lattice dipole_kepler(integrator::core::Particle& p, std::string name) {
  using namespace integrator::core;
  using namespace element;

  double R0 = 1;
  double alpha = 22.5*M_PI/180;
  double L = alpha*R0;
  double By = p.Pc()/CLIGHT/R0*1e6;
  
  Lattice sector(name + "_sector");
  sector = {new MDipole(p, L, By, "MD")};
  return sector.replicate(16);
}

integrator::core::Lattice WF_kepler(integrator::core::Particle& p, std::string name) {
  using namespace integrator::core;
  using namespace element;
  using namespace std;

  double R0 = 1;
  double alpha = 22.5*M_PI/180;
  double L = alpha*R0;

  double gamma = p.gamma();
  double mass0_kg = p.mass0_kg();
  double v = p.beta() * CLIGHT;

  double Ex = gamma*mass0_kg*v*v/(EZERO*R0);
  
  Lattice sector(name + "_sector");
  sector = {new WFCylindrical(p, L, .05, Ex, 0, "WF")};
  return sector.replicate(16);
  
}


int main(int argc, char** argv) {
  using namespace integrator::core;
  using namespace std;

  double mass = 931.494;
  double gamma = (mass + 1)/mass;
  Particle p(mass, gamma, 0);

  int nstate = atoi(argv[1]);
  State::MatrixState proto(nstate, State::VAR_NUM);
  proto.col(0) = Eigen::ArrayXd::LinSpaced(nstate, .07*0, .7);
  State state(proto);
  State ini(state);

  Lattice kepler = dipole_kepler(p, "dipole_kepler");

  int num_turns = atoi(argv[2]);
  data_log::DataLog log;
  clock_t t;
  t = clock();
  pair<size_t, size_t> turn_eid = kepler.track_through(state, log, num_turns);
  double dt = (double)(clock()-t)/CLOCKS_PER_SEC;

  cout << "Turn: " << turn_eid.first
       << "\t EID: " << turn_eid.second << endl;

  cout << scientific << setprecision(3);
  cout << "Tracking took: " << dt << " secs in total\n"
       << dt/num_turns << " per turn \n"
       << "for " << state.count() << " states" << endl;
  cout << "That is, " << dt/num_turns/kepler.element_count() << " secs per element" << endl;
  cout << "# elements: " << kepler.element_count() << endl;

  State delta = state - ini;
  double norm_delta = State::norm(delta);
  
  cout << "ini state: " << ini << endl;
  cout << "fin state: " << state << endl;
  cout << "error = " << norm_delta << endl;

  log.plot("x","s", 0, "points");
}


