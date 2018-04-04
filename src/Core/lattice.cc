// TODO:
//     * when insert_element, update segment_map

#include "Core/lattice.h"
#include <random>
// #include <math.h>
#include <typeinfo>
#include <time.h>

using Gauss = std::normal_distribution<double>;

using namespace std;
using namespace integrator;
using namespace integrator::data_log;
using namespace integrator;
using namespace integrator::element;

Lattice::Lattice(string name)
  : name_(name), length_(0), rf_metadata_(), state_(0){}

Lattice& Lattice::operator=(initializer_list<Element*> element_sequence){

  for (initializer_list<Element*>::iterator element=element_sequence.begin();
       element!=element_sequence.end();
       ++element)
    this->append_element(*element);
    
  return (*this);

}

Lattice& Lattice::operator+=(Lattice& other){ // TODO: I want const here
  cout << "Inside operator+=" << endl;
  // checking for RF in other
  if (other.rf_metadata_.index != -1){
    cout << "Trying to add a lattice segment with an RF;"
	 << "denied" << endl;
    return *this;
  }
  // appending the new elements
  sequence_.reserve(this->element_count() + other.element_count());
  sequence_.insert(this->end(), other.begin(), other.end());
  // updating lattice length
  length_ += other.length_;
  return *this;
}

Lattice& Lattice::replicate(size_t repeat_factor){
  sequence_.reserve(this->element_count()*repeat_factor); // preallocate memory
  Lattice copy(*this); // save the basic state
  for (size_t i=1; i<repeat_factor; i++){
    sequence_.insert(this->end(), copy.begin(), copy.end());
  }
  length_ *= repeat_factor;
  return *this;
}

void Lattice::append_element(Element* new_element){
  // if(new_element->is_RF()){
  //   cout << "Trying to append an RF element; \n"
  // 	 << "please use insert_RF()" << endl;
  //   return;
  // }
  sequence_.push_back(new_element);
  length_ += new_element->length();
  // TODO: update segment_map
}

bool Lattice::insert_element(Element* new_element, int index){
  // if(new_element->is_RF()){
  //   cout << "Trying to insert an RF element; \n"
  // 	 << "please use insert_RF()" << endl;
  //   return false;
  // }
  Lattice::element_iterator position = this->begin()+index;
  // sanity check for segfault
  if (position < this->begin() || position > this->end()){
    cout << "Trying to insert outside lattice bounds" << endl;
    return false;
  }
  length_ += new_element->length();
  sequence_.insert(position, new_element);
  // TODO: update segment_map

  return true;
}

bool Lattice::replace_element(Element* new_element, int index){
  Lattice::element_iterator old_element = this->begin()+index;
  // if(new_element->is_RF() && !old_element->is_RF()){
  //   cout << "Trying to replace an RF element; \n"
  // 	 << "please use insert_RF()" << endl;
  //   return false;
  // }
  length_ -= old_element->length();
  sequence_.replace(old_element, new_element);
  length_ += new_element->length();
  return true;
}

bool Lattice::remove_element(int index) {
  Lattice::element_iterator position = this->begin()+index;
  // sanity check for segfault
  if (position < this->begin() || position > this->end()){
    cout << "Position out of bounds; no element removed" << endl;
    return false;
  }
  Lattice::element_carry element = sequence_.release(position); // ownership transferred to element **
  length_ -= element->length();

  if(element->is_RF()){
    cout << "Removing an RF" << endl;
    rf_metadata_.reset(); // we have only 1 RF element; if it's removed, reset metadata
  }
  
  element.reset(); // hence need delete (called in reset) ** 
  return true;
}

bool Lattice::insert_RF(int index, Particle& reference, RFPars rf_pars){
  // checking for existing RF
  if (this->contains_RF() && index != rf_metadata_.index){
    cout << "Trying to add a second RF element; \n"
	 << "current RF position is " << rf_metadata_.index
	 << endl;
    return false;
  }

  double new_lattice_length = this->length_ + rf_pars.length;
  bool success;

  // if trying to insert an new RF at the place of an existing RF,
  // replace the existing RF with the new one
  if (index == rf_metadata_.index) {
    cout << "Replacing RF: \n"
	 << *(this->begin()+rf_metadata_.index)
	 << endl;
    success = this->replace_element(new ERF(reference, new_lattice_length, rf_pars), index);
    if(success) // if replaced successfully, update rf_w_freq_
      rf_metadata_.w_freq = this->get_RF().w_freq();
    return success;
  }

  success = this->insert_element(new ERF(reference, new_lattice_length, rf_pars), index);
  if (success){
    rf_metadata_.index = index;
    rf_metadata_.w_freq = this->get_RF().w_freq(); // update rf_w_freq_
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
  for (Lattice::element_iterator element=this->begin();
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
  for (Lattice::element_iterator element=this->begin();
       element!=this->end();
       ++element)
    element->clear_tilt();
  state_ = 0;
}

pair<size_t, size_t> Lattice::track_through(State& ini_state, DataLog& log, size_t num_turns){
  // adapting the element vectorized_fields to ini_state size
  for(Lattice::element_iterator element=this->begin();
      element!=this->end();
      ++element){
    element->vectorize_fields(ini_state);
    element->set_RF_w_freq(this->rf_w_freq());
  }
  
  size_t turn, eid; // eid = element id = order of element in lattice
  double current_s = 0;
  bool go_on;
  MetaData metadata(0, "START", 0); // initial state meta data
  log(ini_state, current_s, metadata); // logging initial state
  //tracking proper
  Lattice::element_iterator element;
  int old_percent=-1, percent;
  time_t t;
  for (turn=1; turn<=num_turns; turn++){
    current_s = 0;
    for(element=this->begin(), eid=1;
	element!=this->end();
	++element, eid++){
      // t=clock();
      element->track_through(ini_state); // track through the current element
      // t=clock()-t;
      // cout << element->name() << " track_through " << (float)t/CLOCKS_PER_SEC << endl;

      // t=clock();

      go_on = !ini_state.has_NaN(); // if state has NaNs, stop tracking

      current_s += element->length(); // current position s of the beam along the optical axis
      metadata.overwrite(turn, element->name(), eid); // overwrite metadata for current turn, element
      log(ini_state, current_s, metadata); // logging current state

      if (!go_on)
	return pair<size_t, size_t>(turn, eid);

      // t=clock()-t;
      // cout << "\t rest: " << (float)t/CLOCKS_PER_SEC << endl;
    } // loop over elements
    percent = ((double)turn-1)/num_turns*100;
    if (percent/10 != old_percent/10){
      cout << "Complete: "<< percent << "%" << endl;
      old_percent = percent;
    }
  } // loop over turns
  return pair<size_t, size_t>(turn-1, eid-1);
}
