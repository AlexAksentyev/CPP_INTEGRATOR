// defined classes: Field, Tilt, Element
// Element contains the E-, B-field definitions.

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>

typedef std::vector<double> state_type;


class Element{

  double curve_;
  double length_;
  std::string name_;
  
  double E_field_ [3] = {0, 0, 0};
  double B_field_ [3] = {0, 0, 0};
  

 public:
  Element(double curve, double length, std::string name="Element");
  double curve(){return curve_;}
  double length(){return length_;}
  std::string name(){return name_;}
  
 
};


#endif
