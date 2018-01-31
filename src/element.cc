#include "element.h"
#include <iostream>

using namespace std;

Element::Element(double curve, double length, std::string name)
  : curve_(curve), length_(length), name_(name){
  
}

void Element::print_fields(){
  cout << "(Ex, Ey, Es) = ("
       << E_field_[0] << ","
       << E_field_[1] << ","
       << E_field_[2] << ")\n";

  cout << "(Bx, By, Bs) = ("
       << B_field_[0] << ","
       << B_field_[1] << ","
       << B_field_[2] << ")\n";
}

