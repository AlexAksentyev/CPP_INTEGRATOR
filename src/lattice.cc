#include "lattice.h"

using namespace std;
Lattice::Lattice(string name)
  : name_(name), length_(0), rf_metadata_(){}

void Lattice::add_element(Element* new_element){
  this->push_back(new_element);
  this->length_ += new_element->length();
}

Lattice& Lattice::operator=(initializer_list<Element*> element_sequence){

  for (initializer_list<Element*>::iterator element=element_sequence.begin();
       element!=element_sequence.end();
       ++element)
    this->add_element(*element);
    
  return (*this);

}
