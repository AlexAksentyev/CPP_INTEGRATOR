#ifndef WIEN_FILTER_H
#define WIEN_FILTER_H

#include "element.h"
#include <eigen3/Eigen/Dense>

namespace element{
  struct WFPars {
    double length, h_gap, E_field, B_field;
    std::string name;

    WFPars(double length, double h_gap, double E_field, double B_field, std::string name)
      : length(length), h_gap(h_gap), E_field(E_field), B_field(B_field), name(name) {}
  };
  
  class WFStraight : public Element {
    double ref_kinetic_energy_;
    double kick_voltage_;
  public:
    WFStraight(Particle& reference_particle,
	       double length, double h_gap,
	       double E_hor, double B_vert,
	       std::string name="WFS");

    WFStraight(Particle& reference_particle, WFPars wfpars)
      : WFStraight(reference_particle,
		   wfpars.length, wfpars.h_gap,
		   wfpars.E_field, wfpars.B_field,
		   wfpars.name) {}

    void front_kick(rhs::State& state);
    void rear_kick(rhs::State& state);
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

    VectorizedField EField(rhs::State state);
    void front_kick(rhs::State& state);
    void rear_kick(rhs::State& state);
  };
} // element namespace

#endif // WIEN_FILTER_H
