#include "lattice.h"

using namespace std;
Lattice::Lattice(string name)
  : name_(name), length_(0), rf_metadata_(){}

void Lattice::add_element(Element* new_element){
  this->push_back(new_element);
  this->length_ += new_element->length();
}
