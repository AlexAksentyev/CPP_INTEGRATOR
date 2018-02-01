// defined classes: Field, Tilt, Element
// Element contains the E-, B-field definitions.

// TODO:
//   * add members: tilt, bool_skip (possibly)
//   * add a print method to print the characteristics of an element

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <math.h>
#include <boost/shared_ptr.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>


#include "right_hand_side.h"

struct Tilt{

  Eigen::Affine3d transform_;

  Tilt() : transform_(Eigen::Matrix3d::Identity(3, 3)){}

  void operator() (std::vector<std::pair<char, double>> axis_degangle,
  		   bool append=false);
  
  
};


class Element{

  double curve_;
  double length_;
  std::string name_;
  
  Eigen::Vector3d E_field_base_;
  Eigen::Vector3d B_field_base_;
  Eigen::Matrix<double, 3, Eigen::Dynamic> E_field_vectorized_;
  Eigen::Matrix<double, 3, Eigen::Dynamic> B_field_vectorized_;

  
  

 public:

  Tilt tilt_;

  void vectorize_fields(state_type state_matrix); // public for now, might move
  
  Element(double curve, double length, std::string name="Element");
  double curve(){return curve_;}
  double length(){return length_;}
  std::string name(){return name_;}

  
  // void set_E_field(double, double, double);
  void print_fields(); // for testing purposes
  void print_vectorized_fields(); // testing

  Eigen::MatrixXf EField(state_type state_matrix);
  Eigen::MatrixXf BField(state_type state_matrix);

  void front_kick(state_type state_matrix);
  void rear_kick(state_type state_matrix);

  
 
};


#endif
