// TODO:
//     * when insert_element, update segment_map

#include "lattice.h"
#include <random>
#include <math.h>
#include <boost/shared_ptr.hpp>

using Gauss = std::normal_distribution<double>;

using namespace std;
Lattice::Lattice(string name)
  : name_(name), length_(0), rf_metadata_(), state_(0){}

Lattice& Lattice::operator=(initializer_list<Element*> element_sequence){

  for (initializer_list<Element*>::iterator element=element_sequence.begin();
       element!=element_sequence.end();
       ++element)
    this->append_element(*element);
    
  return (*this);

}

void Lattice::append_element(Element* new_element){
  this->push_back(new_element);
  this->length_ += new_element->length();
  // TODO: update segment_map
}

bool Lattice::insert_element(Element* new_element, int index){
  Lattice::iterator position = this->begin()+index;
  // sanity check for segfault
  if (position > this->end()){
    cout << "Trying to insert outside lattice bounds" << endl;
    return false;
  }
  this->insert(position, new_element);
  length_ += new_element->length();
  // TODO: update segment_map

  return true;
}

bool Lattice::replace_element(Element* new_element, int index){
  Lattice::iterator old_element = this->begin()+index;
  this->replace(old_element, new_element);
}

bool Lattice::remove_element(int index) {
  Lattice::iterator position = this->begin()+index;
  // sanity check for segfault
  if (position < this->begin() || position > this->end()){
    cout << "Position out of bounds; no element removed" << endl;
    return false;
  }
  Lattice::auto_type element = this->release(position); // ownership transferred to element **
  length_ -= element->length();
  delete &element; // hence need delete **
  return true;
}

bool Lattice::insert_RF(int index, Particle& reference, RFPars rf_pars){
  // checking for existing RF
  if (rf_metadata_.count > 1 && index != rf_metadata_.index){
    cout << "Trying to add a second RF element; \n"
	 << "current RF position is " << rf_metadata_.index
	 << endl;
    return false;
  }

  double new_lattice_length = this->length_ + rf_pars.length;
  
  if (index == rf_metadata_.index) {
    cout << "Replacing RF: \n";
    ((*this)[rf_metadata_.index]).print();
    cout  << endl;
    return this->replace_element(new ERF(reference, new_lattice_length, rf_pars), index);
  }

  bool success = this->insert_element(new ERF(reference, new_lattice_length, rf_pars), index);
  if (success){
    rf_metadata_.index = index;
    rf_metadata_.count += 1;
  }
  return success;
}

void Lattice::tilt(vector<boost::tuple<char, double, double>> axis_mean_sigma,
		   bool append) {

  using Triplet = vector<boost::tuple<char, double, double>>;
  
  default_random_engine generator;
  Gauss standard_gauss(0, 1);

  char axis;
  double mean, sigma, tiltangle;
  vector<pair<char, double>> axis_degangle;
  for (Lattice::iterator element=this->begin();
       element!=this->end();
       ++element){ // for this element
    for(Triplet::iterator tilt=axis_mean_sigma.begin();
	tilt!=axis_mean_sigma.end();
	++tilt){ // we have a number of directions to tilt the element about
      axis = tilt->get<0>(); // axis of rotation
      mean = tilt->get<1>();
      sigma = tilt->get<2>();
      // compute the random tilt angle
      tiltangle = standard_gauss(generator)*sigma + mean; // in units of sigma and mean
      // append the element tilt argument
      axis_degangle.push_back(make_pair(axis, tiltangle));
    }
    element->tilt(axis_degangle, append);
    axis_degangle.clear();
  }
  state_ += 1;
}

void Lattice::clear_tilt(){
  for (Lattice::iterator element=this->begin();
       element!=this->end();
       ++element)
    element->tilt.clear();
  state_ = 0;
}
