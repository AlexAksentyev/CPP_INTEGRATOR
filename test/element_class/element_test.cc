
#include <fstream>
#include <iostream>
#include "particle.h"
// #include "element.h"
#include <string>
#include <stdlib.h>
#include <vector>

// #include <eigen3/Eigen/Dense>
//#include <Eigen/Core>
#include <boost/numeric/odeint.hpp>
//#include <boost/numeric/odeint/algebra/vector_space_algebra.hpp>
#include "boost/numeric/odeint/external/eigen/eigen.hpp"

const int VAR_NUM = 5;


using state_type = Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;


struct DataLog{
  std::vector<state_type>& system_state_;
  std::vector<double>& system_position_;
  
  DataLog(std::vector<state_type>& states, std::vector<double>& positions)
  : system_state_(states), system_position_(positions){}
  
  void operator() (const state_type &state, double position){
    system_state_.push_back(state);
    system_position_.push_back(position);
  }

};


class Element_test {
  double fric_;
public:
  Element_test(double fric): fric_(fric){}

  void operator() (const state_type& x, state_type& dxds, const double /* s */){
    // std::cout << x.rows() << " " << x.cols() << std::endl;
    // std::cout << dxds.rows() << " " <<dxds.cols() << std::endl;
    // for (int i=0; i<x.rows(); i++){
    //   dxds(i, 0) = x(i, 1);
    //   dxds(i, 1) = -x(i, 0) - fric_*x(i, 1);
    // }
    dxds.col(0) = x.col(1);
    dxds.col(1) = -x.col(0) - fric_*x.col(1);
  }

  size_t integrate(state_type ini_state, double time_stop, DataLog& log){
    using namespace boost::numeric::odeint;
     runge_kutta_dopri5<state_type, double,
		     state_type, double,
		     vector_space_algebra> stepper;
     size_t num_steps = integrate_adaptive(stepper, *this, ini_state, 0., time_stop, .1, log);

     return num_steps;
  }

};

// void rhs(const state_type& x, state_type& dxds, const double /* s */){
//   dxds[0] = x[1];
//   dxds[1] = -x[0] - 1e-4*x[1];
// }


int main(int argc, char** argv){
  using namespace std;

  double time_stp = atof(argv[1]);
  double coef = atof(argv[2]);

  string ROOT_DIR = "/home/alexa/REPOS/CPP_INTEGRATOR/test/element_class";
 
  // Particle p;
  // Element e(0, 25e-2, p);

  // creating the state ensemble
  int num_states = 2;
  state_type state(num_states, VAR_NUM), deriv(num_states, VAR_NUM);
  state.setZero();
  state.col(0) = Eigen::VectorXd::Ones(num_states);
  // for(int i=0; i<num_states; i++)
  //   for(int j=0; j<VAR_NUM; j++)
  //     state(i,j) = j;

  // deriv.setZero();
  // cout << "initial derivatives: \n "  << deriv << endl;
  cout << "state: \n" << state << endl;
  // e(state, deriv, 0);
  // cout << "computed deriv: \n " << deriv << endl;

  // using namespace boost::numeric::odeint;
  vector<state_type> x;
  vector<double> s;
  DataLog log(x, s);
  Element_test e1(coef);

  // runge_kutta_dopri5<state_type, double,
  // 		     state_type, double,
  // 		     vector_space_algebra> stepper;

  // state_type state(2);
  // state[0] = 1.0; // start at x=1.0, p=0.0
  // state[1] = 0.0;
  // size_t num_steps = integrate_adaptive(stepper, e1, state, 0., time_stp, .1, log);

  size_t num_steps = e1.integrate(state, time_stp, log);

  cout << num_steps << endl;

  ofstream data_file;
  data_file.open((ROOT_DIR+"/data/integrate.dat").c_str());
  int col_width = 10;
  data_file << fixed << setprecision(4);
  data_file << right
  	    << setw(col_width) << "#time"
  	    << setw(col_width) << "x"
  	    << setw(col_width) << "v"
  	    << endl;
  for (size_t i=0; i<num_steps; i++){
    data_file << right
  	      << setw(col_width) << s[i]
  	      << setw(col_width) << x[i](0,0)
  	      << setw(col_width) << x[i](0,1)
  	      << endl;
  }

  data_file.close();

  return 0;
}



