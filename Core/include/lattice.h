
// TODO:
//    * add segment_map functionality
//    * checking for multiple RF elements


#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/tuple/tuple.hpp>

#include "element.h"
#include "rf_element.h"
#include "right_hand_side.h" // provides rhs::State typedef

namespace integrator{
  using ElementPtrVector = boost::ptr_vector<element::Element>;

  struct RFMeta {
    int index;
    double w_freq;
    RFMeta() : index(-1), w_freq(0){};
    RFMeta(const RFMeta& data): index(data.index), w_freq(data.w_freq) {}
    void reset() {index = -1; w_freq = 0;}
    void print() {std::cout << "index: " << index
			    << "frequency (rad/s): " << w_freq
			    << std::endl;}
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
    Lattice(const Lattice& lattice)
      : ElementPtrVector(lattice),
	name_(lattice.name_), length_(lattice.length_),
	rf_metadata_(lattice.rf_metadata_), state_(lattice.state_){}
    
    Lattice& operator=(std::initializer_list<element::Element*>);
    Lattice& operator +=(const Lattice& lattice){
      this->resize(this->size() + lattice.size());
      for(Lattice::iterator element=lattice.begin();
	  element!=lattice.end();
	  ++element)
	this->append_element(element);
      return (*this);
    }

    Lattice& replicate(size_t repeat_factor); 

    double length() {return length_;}

    void append_element(element::Element*); // adds element to back
    // returns true if success
    bool insert_element(element::Element*, int index);
    bool replace_element(element::Element*, int index);
    bool remove_element(int index);
    bool insert_RF(int index, Particle& reference, element::RFPars rf_pars);

    int rf_w_freq(){return rf_metadata_.w_freq;}
    element::ERF& get_RF(){ // dereference iterator and downcast 
      return dynamic_cast<element::ERF&> (*(this->begin()+rf_metadata_.index));
      // SAFETY WARNING: not catching std::bad_cast&
    }
    bool contains_RF(){return rf_metadata_.index != -1? true : false;}
  
    void tilt(std::vector<boost::tuple<char, double, double>> axis_mean_sigma,
	      bool append=false);
    void clear_tilt(); // reset the lattice to the original state

    // data_log::DataLog passed here doesn't go to element::track_through,
    // and only logs the state after passing through the element
    std::pair<size_t, size_t> track_through(rhs::State&, data_log::DataLog&, size_t number_of_turns);

    // methods from base class open to the user
    using iterator = ElementPtrVector::iterator;
    iterator begin() {return this->ElementPtrVector::begin();}
    iterator end() {return this->ElementPtrVector::end();}
  
    element::Element& operator[](size_type n) {return this->ElementPtrVector::operator[](n);}

  };

  inline Lattice operator+(Lattice lhs, const Lattice& rhs){
    lhs += rhs;
    return lhs;
  }
} // namespace integrator

#endif //LATTICE_H
