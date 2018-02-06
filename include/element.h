// defined classes: Field, Tilt, Element
// Element contains the E-, B-field definitions.

// TODO:
//   * add members: bool_skip (possibly)

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>


#include "right_hand_side.h"

using vectorized_field_type = Eigen::Matrix<double, 3, Eigen::Dynamic>;

class Tilt{

  Eigen::Vector3d axis(char name);
  
public:
  Eigen::Affine3d transform_;

  Tilt() : transform_(Eigen::Matrix3d::Identity(3, 3)){}

  void operator() (std::vector<std::pair<char, double>> axis_degangle,
  		   bool append=false);

  
};

class Element : public RightHandSide{

  double curve_;
  double length_;
  std::string name_;

protected:
  Eigen::Vector3d E_field_base_;
  Eigen::Vector3d B_field_base_;
  vectorized_field_type E_field_vectorized_;
  vectorized_field_type B_field_vectorized_;

public:

  Tilt tilt_;

  void vectorize_fields(state_type state_matrix); // public for now, might move
  
  Element(double curve, double length, Particle& particle, std::string name="Element");

  double curve(){return curve_;}
  double length(){return length_;}
  std::string name(){return name_;}

  void print_fields(); // for testing purposes
  void print_vectorized_fields(); // testing

  vectorized_field_type EField(state_type state_matrix);
  vectorized_field_type BField(state_type state_matrix);

  void front_kick(state_type& state_matrix);
  void rear_kick(state_type& state_matrix);

  void print();

  size_t track_through(state_type ini_states, DataLog& observer);
 
};


#endif
