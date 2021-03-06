#include "particle.h"

using namespace std;
using namespace Eigen;

Particle::Particle(double mass0, double gamma, double G)
  : mass0_(mass0), gamma_(gamma), G_(G){

  mass0_kg_ = mass0_*EZERO*1e6/(CLIGHT*CLIGHT);
  kin_nrg_0_ = mass0_*(gamma_ - 1);
}

double Particle::mass0(){return mass0_;}
double Particle::mass0_kg(){return mass0_kg_;}
double Particle::G(){return G_;}

double Particle::kinetic_energy(double relative_dK){
  return kin_nrg_0_*(1 + relative_dK);
}

VariableCol Particle::kinetic_energy(VariableCol relative_dK){
  VariableCol I = VariableCol::Ones(relative_dK.rows());
  return kin_nrg_0_*(I + relative_dK);
}

void Particle::set_kinetic_energy(double value){
  // sanity check
  if (value < 0){
    cout << "Negative energy!" << endl;
    return;
  }

  kin_nrg_0_ = value;
  gamma_ = 1 + value/mass0_;
}

double Particle::gamma (double relative_dK){
  double kin_nrg = kinetic_energy(relative_dK);
  return kin_nrg/mass0_ + 1;
}

VariableCol Particle::gamma (VariableCol relative_dK){
  VariableCol I = VariableCol::Ones(relative_dK.rows());
  VariableCol kin_nrg = kinetic_energy(relative_dK);
  return kin_nrg/mass0_ + I;
}

void Particle::set_gamma(double value){
  // sanity check
  if(value < 0){
    cout << "Negative gamma!" << endl;
    return;
  }

  gamma_ = value;
  kin_nrg_0_ = mass0_*(value - 1);
}

double Particle::beta(double relative_dK){
  double g = gamma(relative_dK);
  return sqrt(g*g - 1)/g;
}

VariableCol Particle::beta(VariableCol relative_dK){
  VariableCol I = VariableCol::Ones(relative_dK.rows());
  VariableCol g = gamma(relative_dK);
  return sqrt(g*g - I)/g;
}

double Particle::Pc(double relative_dK){
  double kin_nrg = kinetic_energy(relative_dK);
  double tot_nrg = mass0_ + kin_nrg;
  return sqrt(tot_nrg*tot_nrg - mass0_*mass0_);
}

VariableCol Particle::Pc(VariableCol relative_dK){
  VariableCol I = VariableCol::Ones(relative_dK.rows());
  VariableCol kin_nrg = kinetic_energy(relative_dK);
  VariableCol tot_nrg = mass0_*I + kin_nrg;
  return sqrt(tot_nrg*tot_nrg - mass0_*mass0_*I);
}

double Particle::revolution_freq(double lattice_length){
  double v = beta()*CLIGHT;
  return v/lattice_length;
}
