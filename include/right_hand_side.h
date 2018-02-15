// The Right Hand Side of the differential equation
// corresponding to a particular element
// has a pointer to the element for the computation of
// the EM fields


#ifndef RIGHT_HAND_SIDE_H
#define RIGHT_HAND_SIDE_H

#include <vector>
#include <Eigen/Dense>
#include <string>

static const int VAR_NUM = 12;
static const std::string VAR_NAME[] = {"x", "y", "s",
				       "t", "Theta", "H",
				       "px", "py", "dK",
				       "Sx", "Sy", "Sz"};
// variable names
/*   x, y, s, */        // 0,  1,  2
/*   t, Theta, H, */    // 3,  4,  5
/*   px, py, dK, */     // 6,  7,  8
/*   Sx, Sy, Sz */      // 9,  10, 11

using state_type = Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;

class Particle;
class Element;
class DataLog;
class RightHandSide{
  Particle& particle_;
  Element& host_;
  double w_freq_; // for now initialized to 0
  // change when have Lattice and can insert an RF
	
public:
  RightHandSide(Particle& reference_particle, Element& host_element);
  void operator() (const state_type &x , state_type &dxds, const double /* s*/);
  size_t integrate(state_type ini_states, DataLog& observer);
};

// template<typename T>
// using Vector = Matrix<T, Dynamic, 1>;
// template<typename T>
// class variable_col {
// public:
//   Vector<T> data;

//   variable_col(Vector<T> given){
//     int num = given.rows();
//     data.setZero(num);    
//     for (int i=0; i<num; i++)
//       data[i] = given[i];
//   }
  
// };


#endif // RIGHT_HAND_SIDE_H
