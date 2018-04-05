#ifndef OPERATOR_PLUS_H
#define OPERATOR_PLUS_H

#include <iostream>
#include <fstream>
#include <boost/ptr_container/ptr_vector.hpp>
#include "Core/elements/element.h"



// class Element {
//   double x_;
// public:
//   Element(double x) : x_(x) {}
//   Element(const Element& another) : x_(another.x_) {}
//   double x(){return x_;}
// };

// class MQuad : public Element {
//   double y_;
// public:
//   MQuad(double x, double y=0) : Element(x), y_(y) {}
//   double y(){return y_;}
// };

using data_type=core::element::Element;

class Lattice {

  using BaseSeq = boost::ptr_vector<data_type>;
  BaseSeq seq_;
  double length_;

public:
  using element_iterator =  BaseSeq::iterator;
  using size_type = BaseSeq::size_type;

  Lattice(double ell) : length_(ell) {}
  Lattice(const Lattice& another)
    : seq_(another.seq_), length_(another.length_) {}

  double length(){return length_;}
  size_type size(){return seq_.size();}

  element_iterator begin(){return seq_.begin();}
  element_iterator end(){return seq_.end();}

  Lattice& operator=(std::initializer_list<data_type*> element_sequence){
    for (std::initializer_list<data_type*>::iterator element=element_sequence.begin();
	 element!=element_sequence.end();
	 ++element)
      seq_.push_back(*element);
    
    return (*this);
  }
  // void append(data_type* element) {seq_.push_back(element);}
  Lattice& operator+=(Lattice& another);

  friend std::ostream& operator<<(std::ostream& out_stream, Lattice& lattice){
    for(Lattice::element_iterator element=lattice.begin();
	element!=lattice.end();
	++element){
      std::cout << (*element) << std::endl;
    }
    return out_stream;
  }

  friend Lattice& operator<<(Lattice& lhs, Lattice& rhs){
    lhs.seq_.transfer(lhs.end(), *rhs.seq_.release());
    return lhs;
  }
};

Lattice& operator<<(Lattice& lhs, Lattice& rhs);

// inline Lattice operator+(Lattice lhs, Lattice& rhs){
//   lhs += rhs;
//   return lhs;
// }


#endif // OPERATOR_PLUS_H
