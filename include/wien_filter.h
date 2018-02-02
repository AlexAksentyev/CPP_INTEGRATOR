#ifndef WIEN_FILTER_H
#define WIEN_FILTER_H

#include "element.h"
#include "particle.h"
#include <eigen3/Eigen/Dense>

class WienFilterStraight : public Element {
  double h_gap_;
  double ref_kinetic_energy_;
  double kick_voltage_;
public:
  WienFilterStraight(double length, double h_gap, Particle_ptr reference_particle,
		     double E_hor, double B_vert, std::string name="WFS");

  void front_kick(state_type_ptr state);
  void rear_kick(state_type_ptr state);
};


#endif
