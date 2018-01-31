// defined classes: Field, Tilt, Element
// Element contains the E-, B-field definitions.

// TODO:
//   * add members: tilt, bool_skip (possibly)
//   * add a print method to print the characteristics of an element

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <eigen3/Eigen/Dense>
#include <math.h>
#include <boost/shared_ptr.hpp>

using state_type = Eigen::VectorXf;
using state_type_ptr = boost::shared_ptr<state_type>;



class Element;
using Element_ptr = boost::shared_ptr<Element>;

class Element{

  double curve_;
  double length_;
  std::string name_;
  
  Eigen::Vector3f E_field_;
  Eigen::Vector3f B_field_;
  

 public:
  Element(double curve, double length, std::string name="Element");
  double curve(){return curve_;}
  double length(){return length_;}
  std::string name(){return name_;}

  void print_fields(); // for testing purposes

  Eigen::MatrixXf EField(state_type state_vector);
  Eigen::MatrixXf BField(state_type state_vector);

  void front_kick(state_type state_vector);
  void rear_kick(state_type state_vector);

  
 
};


#endif
