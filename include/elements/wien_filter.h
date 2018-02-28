#ifndef WIEN_FILTER_H
#define WIEN_FILTER_H

#include "element.h"
#include <eigen3/Eigen/Dense>

namespace element{
  class WFStraight : public Element {
    double ref_kinetic_energy_;
    double kick_voltage_;
  public:
    WFStraight(Particle& reference_particle,
	       double length, double h_gap,
	       double E_hor, double B_vert,
	       std::string name="WFS");

    void front_kick(RHS::State& state);
    void rear_kick(RHS::State& state);
  };

  class WFCylindrical : public Element {
    static double curv_comp(Particle& reference_particle, double E_hor, double B_vert); // required for constructor
    double ref_kinetic_energy_;
    double plate_voltage_;
    double R1_, R2_;
    double kick_voltage(double x);
  public:
    WFCylindrical(Particle& reference_particle,
		  double length, double h_gap,
		  double E_hor, double B_vert,
		  std::string name="WFS");  

    VectorizedField EField(RHS::State state);
    void front_kick(RHS::State& state);
    void rear_kick(RHS::State& state);
  };
} // element namespace

#endif // WIEN_FILTER_H
