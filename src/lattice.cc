#include "lattice.h"
#include <random>
#include <math.h>

using Gauss = std::normal_distribution<double>;

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

// UNFINISHED		
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
      // tiltangle *= 180./M_PI;
      // append the element tilt argument
      axis_degangle.push_back(make_pair(axis, tiltangle));
    }
    element->tilt(axis_degangle, append);
    axis_degangle.clear();
  }
}
