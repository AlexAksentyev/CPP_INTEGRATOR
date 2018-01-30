#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>

const double CLIGHT = 2.99792458e8; // m/s
const double EZERO = 1.602176462e-19; // Coulomb


class Particle{
  double gamma_;
  double kin_nrg_0_;
  

 public:
  double mass0_;
  double G_;
  double mass0_kg_;

  
  Particle(double mass0=1876.5592,
  	   double gamma=1.14394,
  	   double G=-.142987);

  void print(){std::cout << mass0_ << " " << gamma_ << " " << G_ << std::endl;}
  
};



#endif
