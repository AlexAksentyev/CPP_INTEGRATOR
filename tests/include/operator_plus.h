#ifndef OPERATOR_PLUS_H
#define OPERATOR_PLUS_H

#include <vector>
#include <fstream>

class Lattice : private std::vector<double> {

  double length_;

public:

  Lattice(double ell) : length_(ell) {}
  Lattice(const Lattice& another)
    : std::vector<double>(another), length_(another.length_) {}

  Lattice& operator=(std::initializer_list<double> element_sequence){
    for (std::initializer_list<double>::iterator element=element_sequence.begin();
	 element!=element_sequence.end();
	 ++element)
      this->push_back(*element);
    
    return (*this);
  }
  Lattice& operator+=(const Lattice& another){
    this->reserve(this->size() + another.size());
    this->insert(this->end(), another.begin(), another.end());
    // for(Lattice::iterator element=another.begin();
    // 	element!=another.end();
    // 	++element)
    //   this->push_back(*element);
    return (*this);
  }

  friend std::ostream& operator<<(std::ostream& out_stream, Lattice& lattice){
    using namespace std;
    for(vector<double>::iterator element=lattice.begin();
	element!=lattice.end();
	++element)
      out_stream << *element << endl;
    return out_stream;
  }
};


#endif // OPERATOR_PLUS_H
