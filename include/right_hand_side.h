// The Right Hand Side of the differential equation
// corresponding to a particular element



#ifndef RIGHT_HAND_SIDE_H
#define RIGHT_HAND_SIDE_H

#include <vector>
#include "element.h"
#include "particle.h"
#include <boost/shared_ptr.hpp>

using Element_ptr = boost::shared_ptr<Element>;
using Particle_ptr = boost::shared_ptr<Particle>;

class RightHandSide{
  Element_ptr element_;
  Particle_ptr particle_;
	
public:
  Particle_ptr particle_;
  RightHandSide(Element_ptr host, Particle_ptr reference);
  void operator() (const state_type &x , state_type &dxdt, const double /* t*/);
};

#endif
