#include "particle.h"

Particle::Particle(double mass0, double gamma, double G)
  : mass0_(mass0), gamma_(gamma), G_(G){

  std::cout << "in constructor" << std::endl;
  mass0_kg_ = mass0_*EZERO*1e6/(CLIGHT*CLIGHT);
  kin_nrg_0_ = mass0_*(gamma_ - 1);
}

void Particle::GammaBeta(double& gamma, double& beta, double delta_KNRG){
  double kinetic_energy = kin_nrg_0_*(1 + delta_KNRG);
  gamma = kinetic_energy/mass0_ + 1;
  beta = sqrt(gamma*gamma - 1)/gamma;
}

double Particle::Pc(double delta_KNRG){
  double kinetic_energy = kin_nrg_0_*(1 + delta_KNRG);
  double tot_nrg = mass0_ + kinetic_energy;
  return sqrt(tot_nrg*tot_nrg - mass0_*mass0_);
}

double Particle::revolution_freq(double lattice_length){
  double gamma, beta;
  GammaBeta(gamma, beta, kin_nrg_0_);
  double v = beta*CLIGHT;
  return v/lattice_length;
}
