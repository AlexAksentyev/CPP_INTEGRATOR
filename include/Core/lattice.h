
// TODO:
//    * add segment_map functionality
//    * checking for multiple RF elements
//    * operator += argument const keyword (fails to compile, why?)

#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/tuple/tuple.hpp>

#include "Core/elements/element.h"
#include "Core/elements/rf_element.h"
#include "Core/right_hand_side.h" // provides rhs::State typedef

namespace integrator{
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
  class Lattice {
    using ElementPtrVector = boost::ptr_vector<element::Element>;
    ElementPtrVector sequence_;
    std::string name_;
    double length_;
    RFMeta rf_metadata_;
    int state_; // keeps track of lattice tilt state
    // for outputting data into a separate file
  public:
    
    using element_iterator = ElementPtrVector::iterator;
    using size_type = ElementPtrVector::size_type;
    using element_carry = ElementPtrVector::auto_type;
    
    Lattice(std::string name);
    Lattice(const Lattice& lattice)
      : sequence_(lattice.sequence_),
	name_(lattice.name_), length_(lattice.length_),
	rf_metadata_(lattice.rf_metadata_), state_(lattice.state_){}

    Lattice& operator=(std::initializer_list<element::Element*>);
    friend Lattice& operator<<(Lattice& lhs, Lattice& rhs){
      lhs.sequence_.transfer(lhs.end(), *rhs.sequence_.release());
      lhs.length_ += rhs.length_;
      return lhs;
    }
    Lattice& operator+=(Lattice&); // TODO: I want const here
    element::Element& operator[](size_type n) {return sequence_[n];}
    friend std::ostream& operator<<(std::ostream& out_stream, Lattice& lattice){
      for(Lattice::element_iterator element=lattice.begin();
	  element!=lattice.end();
	  ++element){
	std::cout << (*element) << std::endl;
      }
      return out_stream;
    }

    void rename(std::string new_name){name_ = new_name;}
    Lattice& replicate(size_t repeat_factor); 

    element_iterator begin(){return sequence_.begin();}
    element_iterator end(){return sequence_.end();}

    double length() {return length_;}
    size_type element_count() const {return sequence_.size();}

    void append_element(element::Element*); // adds element to back
    // returns true if success
    bool insert_element(element::Element*, int index);
    bool replace_element(element::Element*, int index);
    bool remove_element(int index);
    bool insert_RF(int index, Particle& reference, element::RFPars rf_pars);

    bool contains_RF(){return rf_metadata_.index != -1? true : false;}
    int rf_w_freq(){return rf_metadata_.w_freq;}
    element::ERF& get_RF(){ // dereference iterator and downcast 
      return dynamic_cast<element::ERF&> (*(this->begin()+rf_metadata_.index));
      // SAFETY WARNING: not catching std::bad_cast&
    }
  
    void tilt(std::vector<boost::tuple<char, double, double>> axis_mean_sigma,
	      bool append=false);
    void clear_tilt(); // reset the lattice to the original state

    // data_log::DataLog passed here doesn't go to element::track_through,
    // and only logs the state after passing through the element
    std::pair<size_t, size_t> track_through(rhs::State&, data_log::DataLog&, size_t number_of_turns);
  };

  inline Lattice operator+(Lattice lhs, Lattice& rhs){
    lhs += rhs;
    return lhs;
  }
} // namespace integrator

#endif //LATTICE_H