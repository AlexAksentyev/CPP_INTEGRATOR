// TODO:
//    * remove vectorization from track_through when implemented Lattice class


#include "Core/elements/element.h"
#include <iostream>
#include <iomanip>
#include <boost/numeric/odeint.hpp>
#include "boost/numeric/odeint/external/eigen/eigen.hpp"

using namespace std;
using namespace integrator::data_log;
using namespace integrator::rhs;
using namespace integrator::element;

Element::Element(Particle& particle, double curve, double length, std::string name)
  : rhs_(particle, *this),
    curve_(curve), length_(length), name_(name),
    E_field_base_(0,0,0), B_field_base_(0,0,0),
    tilt_(), shift_(){}

Element::Element(const Element& to_copy)
  : rhs_(to_copy.rhs_, *this),
    curve_(to_copy.curve_), length_(to_copy.length_),
    name_(to_copy.name_),
    tilt_(to_copy.tilt_), shift_(),
    E_field_base_(to_copy.E_field_base_),
    B_field_base_(to_copy.B_field_base_),
    E_field_vectorized_(to_copy.E_field_vectorized_),
    B_field_vectorized_(to_copy.B_field_vectorized_){}

void Element::vectorize_fields(State state_matrix){
  // when the initial state ensemble is known, run
  // this function before tracking proper
  int state_num = state_matrix.count();
  
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
  return E_field_vectorized_;
}
VectorizedField Element::EField_prime_s(State state_matrix){
  return E_field_prime_s_vectorized_;
}
VectorizedField Element::BField(State state_matrix){
  return B_field_vectorized_;
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

size_t Element::track_through(integrator::State& ini_states, DataLog& observer){
  this->vectorize_fields(ini_states); // remove this later when have class Lattice
  using namespace boost::numeric::odeint;
  runge_kutta_dopri5<integrator::State> stepper;
  double delta_s = length_/100;
  front_kick(ini_states);
  size_t num_steps = integrate_adaptive(stepper, this->rhs_, ini_states, 0., length_, delta_s, observer);
  rear_kick(ini_states);

  return num_steps;
}

size_t Element::track_through(integrator::State& ini_states){
  //  this->vectorize_fields(ini_states); // this is now done in Lattice::track_through
  using namespace boost::numeric::odeint;
  runge_kutta_dopri5<integrator::State> stepper;
  double delta_s = length_/100;
  front_kick(ini_states);
  size_t num_steps=integrate_adaptive(stepper, this->rhs_, ini_states, 0., length_, delta_s);
  rear_kick(ini_states);

  return num_steps;
}


void TiltableElement::tilt(std::vector<std::pair<char, double>> axis_degangle,
	  bool append){
  tilt_(axis_degangle, append);

  double By = B_field_base_[1];
  double dBx = By*tilt_.tan_angle_s();
  double v = ref_beta_*CLIGHT;

  delta_E_field_ << dBx, 0, By*tilt_.tan_angle_x();
  delta_E_field_ << 0, -v*dBx, 0;
}

VectorizedField TiltableElement::BField(State state){
  return Element::BField(state) + delta_B_field_.replicate(1, state.count());
}

VectorizedField TiltableElement::Ey_comp(State state){
  return delta_E_field_.replicate(1, state.count());
}

VectorizedField TiltableElement::EField(State state){
  return Element::EField(state) + Ey_comp(state);
}

