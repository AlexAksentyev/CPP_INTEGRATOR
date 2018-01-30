#include "particle.h"

Particle::Particle(double mass0, double gamma, double G)
  : mass0_(mass0), gamma_(gamma), G_(G){

  std::cout << "in constructor" << std::endl;
  mass0_kg_ = mass0_*EZERO*1e6/(CLIGHT*CLIGHT);
  kin_nrg_0_ = mass0_*(gamma_ - 1);
}
