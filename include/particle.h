#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <Eigen/Dense>

const double CLIGHT = 2.99792458e8; // m/s
const double EZERO = 1.602176462e-19; // Coulomb

using variable_col = Eigen::ArrayXd;

class Particle{
  double gamma_;
  double kin_nrg_0_;

  double mass0_;
  double G_;
  double mass0_kg_;


 public:  
  Particle(double mass0=1876.5592,
  	   double gamma=1.14394,
  	   double G=-.142987);
  

  double mass0();
  double mass0_kg();
  double G();

  double kinetic_energy(double relative_delta_energy=0);
  variable_col kinetic_energy(variable_col relative_delta_energy); // no default because dynamic allocation
  void set_kinetic_energy(double value);
  double gamma(double relative_delta_energy=0);
  variable_col gamma(variable_col relative_delta_energy);
  void set_gamma(double value);
  double beta(double relative_delta_energy=0);
  variable_col beta(variable_col relative_delta_energy);
  double Pc(double relative_delta_energy=0);
  variable_col Pc(variable_col relative_delta_energy);
  
  double revolution_freq(double lattice_length);

  void print(){std::cout << mass0_ << " " << gamma_ << " " << G_ << std::endl;}
  
};

#endif
