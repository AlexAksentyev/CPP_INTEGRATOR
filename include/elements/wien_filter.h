#ifndef WIEN_FILTER_H
#define WIEN_FILTER_H

#include "element.h"
#include "particle.h"
#include <eigen3/Eigen/Dense>


class WFStraight : public Element {
  //  double h_gap_;
  double ref_kinetic_energy_;
  double kick_voltage_;
public:
  WFStraight(Particle& reference_particle,
	     double length, double h_gap,
	     double E_hor, double B_vert,
	     std::string name="WFS");

  void front_kick(state_type& state);
  void rear_kick(state_type& state);
};

class WFCylindrical : public Element {
  static double curv_comp(Particle& reference_particle, double E_hor, double B_vert); // required for constructor
  double ref_kinetic_energy_;
  double plate_voltage_;
  double R1_, R2_;
  variable_col kick_voltage(state_type state);
  double kick_voltage(double x);
public:
  WFCylindrical(Particle& reference_particle,
		double length, double h_gap,
		double E_hor, double B_vert,
		std::string name="WFS");  

  vectorized_field_type EField(state_type state);
  void front_kick(state_type& state);
  void rear_kick(state_type& state);
};


#endif
