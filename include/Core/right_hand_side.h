// The Right Hand Side of the differential equation
// corresponding to a particular element
// has a pointer to the element for the computation of
// the EM fields


#ifndef RIGHT_HAND_SIDE_H
#define RIGHT_HAND_SIDE_H

#include <Eigen/Dense>
#include <string>

#include <Core/state.h>

namespace integrator {
  namespace core {
    class Particle;
    namespace data_log{class DataLog;}
    namespace element{class Element;}

    namespace rhs{

      class RightHandSide{
	Particle& particle_;
	element::Element& host_;
	double w_freq_; // for now initialized to 0
	// change when have Lattice and can insert an RF

      public:
	RightHandSide(Particle& reference_particle, element::Element& host_element);
	RightHandSide(const RightHandSide& to_copy, element::Element& new_host);
	RightHandSide(const RightHandSide& to_copy);
	void operator() (const State &x , State &dxds, const double /* s*/);
	void set_w_freq(double value) {w_freq_ = value;}
      };
    } // namespace RHS
  } // namespace core
} // namespace integrator
#endif // RIGHT_HAND_SIDE_H
