#include "element.h"
#include <iostream>

using namespace std;

Element::Element(double curve, double length, std::string name)
  : curve_(curve), length_(length), name_(name),
    E_field_(0,0,0), B_field_(0,0,0){
  
}

void Element::print_fields(){
  cout << "(Ex, Ey, Es) : \n"
       << E_field_
       << endl;

  cout << "(Bx, By, Bs) : \n"
       << B_field_
       << endl;
}

