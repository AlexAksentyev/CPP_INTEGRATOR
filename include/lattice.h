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
#include<boost/tuple/tuple.hpp>

#include "element.h"
#include "rf_element.h"

using ElementPtrVector = boost::ptr_vector<Element>;

struct RFMeta {
  int index, count;
  RFMeta() : index(-1), count(0) {};
};

class ERF;
class Lattice : private ElementPtrVector {
  std::string name_;
  double length_;
  RFMeta rf_metadata_;
  int state_; // keeps track of lattice tilt state
  // for outputting data into a separate file
public:
  
  Lattice(std::string name);
  Lattice& operator=(std::initializer_list<Element*>);

  double length() {return length_;}

  void append_element(Element*); // adds element to back
  // returns true if success
  bool insert_element(Element*, int index);
  bool replace_element(Element*, int index);
  bool remove_element(int index);
  bool insert_RF(int index, Particle& reference, RFPars rf_pars);
  
  void tilt(std::vector<boost::tuple<char, double, double>> axis_mean_sigma,
	    bool append=false);
  void clear_tilt(); // reset the lattice to the original state

};



#endif //LATTICE_H
