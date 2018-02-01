// The Right Hand Side of the differential equation
// corresponding to a particular element
// has a pointer to the element for the computation of
// the EM fields


#ifndef RIGHT_HAND_SIDE_H
#define RIGHT_HAND_SIDE_H

#include <vector>
#include "element.h"
#include "particle.h"
#include <boost/shared_ptr.hpp>

static const int VAR_NUM = 3; // 3 for testing purposes,
                              // otherwise 12
// variable names
/*   x, y, s, */
/*   t, Theta, H, */
/*   px, py, dK, */
/*   Sx, Sy, Sz */ 

class Element;
using state_type = Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;
using state_type_ptr = boost::shared_ptr<state_type>;

using Element_ptr = boost::shared_ptr<Element>;
using Particle_ptr = boost::shared_ptr<Particle>;

class RightHandSide{
  Element_ptr element_;
  Particle_ptr particle_;
	
public:
  RightHandSide(Element_ptr host, Particle_ptr reference);
  void operator() (const state_type &x , state_type &dxdt, const double /* t*/);
};

#endif
