// defined classes: Field, Tilt, Element
// Element contains the E-, B-field definitions.
// TODO:
//    * data output is performed only inside odeint,
//      but in python code I output values after odeint + rear_kick
//    * copy constructor

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "particle.h"
#include "data_log.h"
#include "right_hand_side.h"
#include "tilt.h"


using VectorizedField = Eigen::Matrix<double, 3, Eigen::Dynamic>;

class Element {

  double curve_;
  double length_;
  std::string name_;
  RightHandSide rhs_;

protected:
  Eigen::Vector3d E_field_base_;
  Eigen::Vector3d B_field_base_;
  VectorizedField E_field_vectorized_;
  VectorizedField E_field_prime_s_vectorized_;
  VectorizedField B_field_vectorized_;

public:

  Tilt tilt;

  void vectorize_fields(State state_matrix); // public for now, might move

  Element(Particle& particle,
	  double curve, double length,
	  std::string name="Element");

  Element(const Element& ); // copy constructor

  virtual bool is_RF() {return false;}

  double curve(){return curve_;}
  double length(){return length_;}
  std::string name(){return name_;}
  void rename(std::string new_name){name_ = new_name;}

  void print_fields(); // for testing purposes
  void print_vectorized_fields(); // testing

  virtual VectorizedField EField(State state_matrix);
  virtual VectorizedField EField_prime_s(State state_matrix);
  virtual VectorizedField BField(State state_matrix);

  virtual void front_kick(State& state_matrix);
  virtual void rear_kick(State& state_matrix);

  void print();
  friend std::ostream& operator<<(std::ostream&, const Element&);

  // tracking with intermediate values output
  virtual size_t track_through(State& ini_states, DataLog& observer);
  // tracking w/o intermediate values output;
  // data logging is handled in the Lattice class' track_through
  virtual size_t track_through(State& ini_states);
 
};

class Observer : public Element {
public:
  Observer(Particle& particle, std::string name="Observer")
    : Element(particle, 0, 0, name){}

  size_t track_through(State& ini_states, DataLog& observer) {
    observer(ini_states, ini_states(0, 2));
    return 0;
  }

};


#endif
