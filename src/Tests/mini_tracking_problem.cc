
#include <iostream>
#include <stdlib.h>
#include <string>
#include <boost/numeric/odeint.hpp>
#include "boost/numeric/odeint/external/eigen/eigen.hpp"
#include <Eigen/Dense>

#include <Utilities/read_matrix.h>
#include <time.h>
#include <Tests/state.h>

const static size_t VAR_NUM = 3;

using VecField = Eigen::Matrix<double, 3, Eigen::Dynamic>;
//using State = Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;
using State = core::State;
// using VarCol = Eigen::ArrayXd;
using VarCol = core::VariableCol;

class RHS {
  double w_;
public:
  RHS(double w) : w_(w) {}

  void operator()(const State& state, State& deriv, const double /*t*/){
    VarCol x = state.var(0);
    VarCol y = state.var(1);
    VarCol dK = state.var(2);
    VarCol xp = y;
    VarCol yp = -w_*w_*x;

    deriv.set_var(0, xp); deriv.set_var(1, yp);
  }
  
};

int main(int argc, char** argv){
  using namespace std;
  cout << "running" << endl;

  string home_dir = getenv("HOME");
  string root_dir = home_dir + "/REPOS/CPP_INTEGRATOR";
  ofstream data_file; data_file.open(root_dir+"/data/mini_tracker_problem_eigen.dat");

  size_t num_state = atoi(argv[1]);
  // State ini_states(num_state, VAR_NUM);
  // ini_states.col(0) = Eigen::VectorXd::LinSpaced(num_state, -1e-3, 1e-3);
  // ini_states.col(2) = Eigen::VectorXd::LinSpaced(num_state, 0, 1e-4);

  RHS rhs(3); 
  clock_t t;

  using namespace boost::numeric::odeint;
  // runge_kutta_dopri5<State, double,
  // 		     State, double> stepper;
  // double length = 1.;
  // double delta_s = length/100;
  // for (int test=0; test < 30; test++){
  //   t=clock();
  //   for(int i=0; i<500; i++)
  //     size_t num_stp = integrate_adaptive(stepper, rhs, ini_states, 0., length, delta_s);
  //   t=clock()-t;
  //   data_file << (float)t/CLOCKS_PER_SEC << endl;
  // }

  data_file.close();
}
