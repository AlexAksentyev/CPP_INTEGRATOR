#define EIGEN_RUNTIME_NO_MALLOC
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <Eigen/Dense>
#include <boost/numeric/odeint.hpp>
#include "boost/numeric/odeint/external/eigen/eigen.hpp"
#include <time.h>
#include <Eigen/src/Core/util/Macros.h>

const static int VAR_NUM = 2;

using State = boost::numeric::ublas::vector<double>;
// std::vector<double>;
// Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;
using VarCol = Eigen::ArrayXd;

class RHS {

  double w_, k_;

public:
  RHS(double wfreq, double kappa): w_(wfreq), k_(kappa) {}

  void operator()(const State& state, State& der, const double t){
    // VarCol f = VarCol::Ones(state.rows()) * cos(w_*.95*t);
    // VarCol xp = state.col(1);
    // double w2 = w_*w_;
    // VarCol yp = w2*(VarCol)state.col(0) + f;


    // der.col(0) = xp; der.col(1) = yp;

    double w2 = w_*w_;
    for (int i=0; i<state.size(); i+=VAR_NUM){
      der[i] = state[i+1]; // y
      der[i+1] = w2*state[i] + cos(w_*.95*t); // w^2*x
    }
    
  }
  
};

int main(int argc, char** argv){
  using namespace std;

  
  size_t num_state = atoi(argv[1]);
  size_t num_reps  = atoi(argv[2]);

  RHS ho(3, 0);

  // State x0(num_state, 2);
  // x0.col(0) = VarCol::LinSpaced(num_state, -.1, .1);
  // x0.col(1) = VarCol::LinSpaced(num_state, .1, -.1);

  State x0(2*num_state);
  double delta = .1/num_state;
  for (int i=0; i<2*num_state; i+=2){
    x0[i] = -.1 + delta*i;
    x0[i+1] = .1 - delta*i;
  }
    
  using namespace boost::numeric::odeint;
  runge_kutta_dopri5<State// , double,
		     // State, double,
		     // vector_space_algebra
		     > stepper;
  size_t steps = 0;

  vector<float> run_times;
  
  clock_t t;
  for (int run=0; run < 30; run++){

    t = clock();
    for (int i=0; i<num_reps; i++)
      steps += integrate_adaptive(stepper, ho, x0, 0., 10., .1);

    run_times.push_back((float)(clock()-t)/CLOCKS_PER_SEC);
  }

  double sum = accumulate(run_times.begin(), run_times.end(), 0.0);
  double mean = sum/ run_times.size();

  double sq_sum = std::inner_product(run_times.begin(), run_times.end(), run_times.begin(), 0.0);
  double stdev = std::sqrt(sq_sum / run_times.size() - mean * mean);

  cout << mean << " +- " << stdev << endl;
  
  return steps;

}
