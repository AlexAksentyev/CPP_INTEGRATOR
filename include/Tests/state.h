// wrapper around std::vector<double> with utilities for
// retrieving  

#ifndef	STATE_H
#define STATE_H

#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <Eigen/Dense>
#include <Utilities/read_matrix.h>

#include <boost/numeric/odeint.hpp> // required for is_resizeable
#include <boost/tuple/tuple.hpp>

namespace integrator {
  namespace core {

    class State {      
    public:
      using Vector6d = Eigen::Matrix<double, 6, 1>;
      using Vector3d = Eigen::Vector3d;
      
      State(): dynamics_(), spin_() {}
      State(Vector6d dynamics, Vector3d spin)
	: dynamics_(dynamics), spin_(spin) {}
      State(const State& other)
	: dynamics_(other.dynamics_), spin_(other.spin_) {}

      Vector6d dynamics() {return dynamics_;}
      Vector3d spin() {return spin_;}

      double& operator()(int i){return dynamics_(i);}
      const double& operator()(int i) const {return dynamics_(i);}

      double& operator[](int i){return spin_[i];}
      const double& operator[](int i) const {return spin_[i];}

      friend std::ostream& operator<<(std::ostream& out, State& state) {
	out << std::scientific;
      	out << "(" << state.dynamics_ << ","
      	    << state.spin_ << ")" << std::endl;
	
      	return out;
      }

    private:
      Vector6d dynamics_;
      Vector3d spin_;

    }; // class State

  } // namespace core
} // namespace integrator

namespace boost { namespace numeric { namespace odeint {
      template<>
      struct is_resizeable<integrator::core::State>
      {
	typedef boost::true_type type;
	static const bool value = type::value;
      };

    } } }

// TO FINISH
// class state_algebra : public boost::numeric::odeint::range_algebra {
//     public:
//         template< typename S >
//         static double norm_inf( const S &s ){
// 	  using std::max_element;
// 	  using integrator::core::abs;
// 	  integrator::core::State abs_s = abs(s);
// 	  return  *max( abs_s.begin(), abs_s.end() );
//         }
// }; // class state_algebra

#endif //STATE_H
