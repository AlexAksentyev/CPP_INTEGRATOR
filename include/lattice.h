// TODO:
//    * add segment_map functionality



#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>
#include <boost/shared_ptr>
#include "element"

using Element_ptr = boost::shared_ptr<Element>;

struct RF_t {
  int index, count;
};

class Lattice {
  std::vector<Element_ptr> sequence_; // contains ponters to used elements

public:
  Lattice(std::vector<Element_ptr> element_sequence, std::string name);

  

};


#endif //LATTICE_H
