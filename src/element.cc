
#include "element.h"
#include <iostream>
#include <boost/numeric/odeint.hpp>
#include "boost/numeric/odeint/external/eigen/eigen.hpp"

using namespace std;

Element::Element(Particle& particle, double curve, double length, std::string name)
  : curve_(curve), length_(length), name_(name),
    E_field_base_(0,0,0), B_field_base_(0,0,0),
    tilt_(){
  
}

void Element::vectorize_fields(state_type state_matrix){
  // when the initial state ensemble is known, run
  // this function before tracking proper
  int state_num = state_matrix.rows();
  
  E_field_vectorized_ = E_field_base_.replicate(1, state_num);
  B_field_vectorized_ = B_field_base_.replicate(1, state_num);
}

void Element::print_fields(){
  cout << "(Ex, Ey, Es) : \n"
       << E_field_base_
       << endl;

  cout << "(Bx, By, Bs) : \n"
       << B_field_base_
       << endl;
}

void Element::print_vectorized_fields(){
  cout << "(Ex, Ey, Es) : \n"
       << E_field_vectorized_
       <<endl;

  cout << "(Bx, By, Bs) : \n"
       << B_field_vectorized_
       << endl;
}

vectorized_field_type Element::EField(state_type state_matrix){
  return tilt_.transform_*E_field_vectorized_;
}
vectorized_field_type Element::BField(state_type state_matrix){
  return tilt_.transform_*B_field_vectorized_;
}

void Element::front_kick(state_type& state_matrix){
  // depends on the element;
  // for magnetic ones it does nothing
}

void Element::rear_kick(state_type& state_matrix){/* same here */}

void Element::print(){
  cout << "curvature: " << curve_ << endl
       << "length: " << length_ << endl
       << "name: " << name_  << endl;
}

size_t Element::track_through(state_type ini_states, DataLog& observer){
  using namespace boost::numeric::odeint;
  runge_kutta_dopri5<state_type, double,
		     state_type, double,
		     vector_space_algebra> stepper;
  double delta_s = .1;
  front_kick(ini_states);
  size_t num_steps = 0; // integrate_adaptive(stepper, *this, ini_states, 0., length_, delta_s, observer);
  rear_kick(ini_states);

  return num_steps;
}

using namespace Eigen;

Vector3d Tilt::axis(char name){
  switch(toupper(name)){
  case 'X': return Vector3d::UnitX();
  case 'Y': return Vector3d::UnitY();
  case 'Z': return Vector3d::UnitZ();
  default: cout << "Wrong axis!" << endl;
  }
}

void Tilt::operator() (vector<pair<char, double>> axis_degangle,
		       bool append){

  Affine3d result;
  if(append)
    result = transform_;
  else
    result.setIdentity();

  Vector3d unit_axis;
  double radangle;
  for(vector<pair<char, double>>::iterator
	it = axis_degangle.begin();
      it != axis_degangle.end();
      ++it){
    unit_axis = axis((*it).first);
    radangle = M_PI/180*(*it).second;
    Affine3d t(AngleAxisd(radangle, unit_axis));
    result = t*result;
  }

  transform_ = result;
  
}
