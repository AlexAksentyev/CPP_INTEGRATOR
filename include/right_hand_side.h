// The Right Hand Side of the differential equation
// corresponding to a particular element
// has a pointer to the element for the computation of
// the EM fields


#ifndef RIGHT_HAND_SIDE_H
#define RIGHT_HAND_SIDE_H

#include <vector>
#include "particle.h"
#include <Eigen/Dense>

static const int VAR_NUM = 12;
// variable names
/*   x, y, s, */
/*   t, Theta, H, */
/*   px, py, dK, */
/*   Sx, Sy, Sz */ 

using state_type = Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;

class RightHandSide{
  Particle& particle_;
	
public:
  RightHandSide(Particle& reference_particle);
  void operator() (const state_type &x , state_type &dxds, const double /* s*/);
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
