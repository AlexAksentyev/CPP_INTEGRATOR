
#include "element.h"
#include <iostream>

using namespace std;

Element::Element(double curve, double length, std::string name)
  : curve_(curve), length_(length), name_(name),
    E_field_base_(0,0,0), B_field_base_(0,0,0){
  
}

void Element::vectorize_fields(state_type state_matrix){
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

