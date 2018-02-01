#include "element.h"



class MQuad : public Element {

  double grad_;
  
public:
  
  MQuad(double length, double gradient, std::string name="MQuad")
    : Element(0, length, name), grad_(gradient) {}

  void print();
};
