#include "lattice.h"

using namespace std;
Lattice::Lattice(ElementVector element_sequence, string name)
  : sequence_(element_sequence), length_(0), count_(element_sequence.size()){

  for(ElementVector::iterator element=element_sequence.begin();
      element != element_sequence.end();
      ++element){
    // checking for RF elements and filling out rf_metadata_
    length_ += element->length();
  }
  
}
