// TODO:
//    * remove vectorization from track_through when implemented Lattice class


#include "element.h"
#include <iostream>
#include <boost/numeric/odeint.hpp>
#include "boost/numeric/odeint/external/eigen/eigen.hpp"

using namespace std;

Element::Element(Particle& particle, double curve, double length, std::string name)
  : rhs_(particle, *this),
    curve_(curve), length_(length), name_(name),
    E_field_base_(0,0,0), B_field_base_(0,0,0),
    tilt_(){}

Element::Element(const Element& to_copy)
  : rhs_(to_copy.rhs_, *this),
    curve_(to_copy.curve_), length_(to_copy.length_),
    name_(to_copy.name_),
    E_field_base_(to_copy.E_field_base_),
    B_field_base_(to_copy.B_field_base_),
    tilt_(to_copy.tilt_),
    E_field_vectorized_(to_copy.E_field_vectorized_),
    B_field_vectorized_(to_copy.B_field_vectorized_){}

void Element::vectorize_fields(State state_matrix){
  // when the initial state ensemble is known, run
  // this function before tracking proper
  int state_num = state_matrix.rows();
  
  E_field_vectorized_ = E_field_base_.replicate(1, state_num);
  E_field_prime_s_vectorized_ = VectorizedField::Zero(3, state_num);
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

VectorizedField Element::EField(State state_matrix){
  return tilt_.transform_*E_field_vectorized_;
}
VectorizedField Element::EField_prime_s(State state_matrix){
  return tilt_.transform_*E_field_prime_s_vectorized_;
}
VectorizedField Element::BField(State state_matrix){
  return tilt_.transform_*B_field_vectorized_;
}

void Element::front_kick(State& state_matrix){
  // depends on the element;
  // for magnetic ones it does nothing
}

void Element::rear_kick(State& state_matrix){/* same here */}

void Element::print(){
  cout << "curvature: " << curve_ << endl
       << "length: " << length_ << endl
       << "name: " << name_  << endl;
}

size_t Element::track_through(State& ini_states, DataLog& observer, double start_s){
  this->vectorize_fields(ini_states); // remove this later when have class Lattice
  using namespace boost::numeric::odeint;
  runge_kutta_dopri5<State, double,
		     State, double,
		     vector_space_algebra> stepper;
  double delta_s = length_/100;
  front_kick(ini_states);
  size_t num_steps = integrate_adaptive(stepper, this->rhs_, ini_states, start_s, start_s+length_, delta_s, observer);
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
