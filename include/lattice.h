// Keep elements instead of element pointers, b/c
// if keep pointers, tilting will be systematic

// TODO:
//    * add segment_map functionality
//    * checking for multiple RF elements


#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include "element.h"

using ElementPtrVector = boost::ptr_vector<Element>;

struct RFMeta {
  int index, count;
  RFMeta() : index(0), count(0) {};
};

class Lattice : public ElementPtrVector {
  std::string name_;
  double length_;
  RFMeta rf_metadata_;
public:
  
  Lattice(std::string name);

  void add_element(Element* );

  double length() {return length_;}

};



#endif //LATTICE_H
