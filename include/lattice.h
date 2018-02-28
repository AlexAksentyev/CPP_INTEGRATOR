
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
#include "right_hand_side.h" // provides State typedef

using ElementPtrVector = boost::ptr_vector<Element>;

struct RFMeta {
  int index, count;
  RFMeta() : index(-1), count(0) {};
  void reset() {index = -1; count = 0;}
  void print() {std::cout << "index: "  << index
 			   << " count: " << count << std::endl;}
};

namespace data_log{
  class DataLog;
}
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

  // data_log::DataLog passed here doesn't go to element::track_through,
  // and only logs the state after passing through the element
  size_t track_through(State, data_log::DataLog&, size_t number_of_turns);

  // methods from base class open to the user
  using iterator = ElementPtrVector::iterator;
  iterator begin() {return this->ElementPtrVector::begin();}
  iterator end() {return this->ElementPtrVector::end();}
  
  Element& operator[](size_type n) {return this->ElementPtrVector::operator[](n);}

};



#endif //LATTICE_H
