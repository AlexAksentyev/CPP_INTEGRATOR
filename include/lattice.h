// Keep elements instead of element pointers, b/c
// if keep pointers, tilting will be systematic

// TODO:
//    * add segment_map functionality
//    * checking for multiple RF elements


#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>
#include "element.h"

using ElementSequence = std::vector<Element>;

struct rf_t {
  int index, count;
};

class Lattice {
  ElementSequence sequence_; // contains ponters to used elements
  double length_;
  size_t count_;
  rf_t rf_metadata_;
public:
  
  Lattice(ElementSequence element_sequence, std::string name);

  size_t count() {return count_;}
  double length() {return length_;}

};


#endif //LATTICE_H
