#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <Eigen/Dense>

namespace integrator {
  const double CLIGHT = 2.99792458e8; // m/s
  const double EZERO = 1.602176462e-19; // Coulomb

  using VariableCol = Eigen::ArrayXd;

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
    VariableCol kinetic_energy(VariableCol relative_delta_energy); // no default because dynamic allocation
    void set_kinetic_energy(double value);
    double gamma(double relative_delta_energy=0);
    VariableCol gamma(VariableCol relative_delta_energy);
    void set_gamma(double value);
    double beta(double relative_delta_energy=0);
    VariableCol beta(VariableCol relative_delta_energy);
    double Pc(double relative_delta_energy=0);
    VariableCol Pc(VariableCol relative_delta_energy);
  
    double revolution_freq(double lattice_length);

    void print(){std::cout << mass0_ << " " << gamma_ << " " << G_ << std::endl;}
  };
} // namespace integrator

#endif
