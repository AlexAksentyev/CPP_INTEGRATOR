#ifndef OPERATOR_PLUS_H
#define OPERATOR_PLUS_H

//#include <vector>
#include <fstream>
#include <boost/ptr_container/ptr_vector.hpp>

class Lattice : private boost::ptr_vector<double> {

  double length_;

public:

  Lattice(double ell) : length_(ell) {}
  Lattice(const Lattice& another)
    : boost::ptr_vector<double>(another), length_(another.length_) {}

  double length(){return length_;}

  Lattice& operator=(std::initializer_list<double*> element_sequence){
    for (std::initializer_list<double*>::iterator element=element_sequence.begin();
	 element!=element_sequence.end();
	 ++element)
      this->push_back(*element);
    
    return (*this);
  }
  Lattice& operator+=(const Lattice& another);

  friend std::ostream& operator<<(std::ostream& out_stream, Lattice& lattice){
    for(boost::ptr_vector<double>::iterator element=lattice.begin();
	element!=lattice.end();
	++element)
      out_stream << *element << std::endl;
    return out_stream;
  }
};


#endif // OPERATOR_PLUS_H
