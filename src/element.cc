
#include "element.h"
#include <iostream>

using namespace std;

Element::Element(double curve, double length, std::string name)
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
  return E_field_vectorized_; // don't tilt because the default
  // field is 0 anyway;
  // tilting is added in derived elements
}
vectorized_field_type Element::BField(state_type state_matrix){
  return B_field_vectorized_; // same here
}

void Element::front_kick(state_type state_matrix){
  // depends on the element;
  // for magnetic ones it does nothing
}

void Element::rear_kick(state_type state_matrix){/* same here */}

void Element::print(){
  cout << "curvature: " << curve_ << endl
       << "length: " << length_ << endl
       << "name: " << name_  << endl;
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
