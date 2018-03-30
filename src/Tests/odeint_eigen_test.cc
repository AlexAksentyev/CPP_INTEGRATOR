// #define EIGEN_RUNTIME_NO_MALLOC
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <Eigen/Dense>
#include <boost/numeric/odeint.hpp>
// #include "boost/numeric/odeint/external/eigen/eigen.hpp"
#include <time.h>
// #include <Eigen/src/Core/util/Macros.h>

#include <Core/state.h>
using namespace integrator;

// const static int VAR_NUM = 2;

// class State {
//   using value_type = double;
//   using data_container = std::vector<value_type>;
//   data_container data_;

// public:
//   using iterator = data_container::iterator;
//   using const_iterator = data_container::const_iterator;
//   using size_type = data_container::size_type;

//   State() {}
//   State(size_type n): data_(n) {}
//   State(const State& other): data_(other.data_) {}

//   value_type& operator[](int index) {return data_[index];}
//   const value_type& operator[](int index) const {return data_[index];}
//   size_type size() const {return data_.size();}
//   void resize(size_type n) {data_.resize(n);}
//   void resize (size_type n, const value_type& val) {data_.resize(n, val);}

//   iterator begin(){return data_.begin();}
//   iterator end(){return data_.end();}
//   const_iterator begin() const {return data_.begin();}
//   const_iterator end() const {return data_.end();}

// };


// namespace boost { namespace numeric { namespace odeint {
//       template<>
//       struct is_resizeable<State>
//       {
// 	typedef boost::true_type type;
// 	static const bool value = type::value;
//       };

//     } } }

// using State = std::vector<double>;
// // Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;
// using VarCol = Eigen::ArrayXd;
using VarCol = integrator::VariableCol;

class RHS {

  double w_, k_;

public:
  RHS(double wfreq, double kappa): w_(wfreq), k_(kappa) {}

  void operator()(const State& state, State& der, const double t){
    // VarCol f = VarCol(state.count(), 1) * cos(w_*.95*t);
    // VarCol xp = state.var(1);
    // double w2 = w_*w_;
    // VarCol yp = w2*state.var(0) + f;

    // der.set(0, xp); der.set(1, yp);

    double w2 = w_*w_;
    for (int i=0; i<state.count(); i++){
      der(i, 0) = state(i, 1); // y
      der(i, 1) = w2*state(i, 0) + cos(w_*.95*t); // w^2*x
    }
    
  }
  
};

// int main(int argc, char** argv){
//   using namespace std;
  
//   size_t num_state = atoi(argv[1]);
//   size_t num_reps  = atoi(argv[2]);

//   State x0(2*num_state);
//   double delta = .1/(num_state-1);
//   for (int i=0; i<2*num_state; i+=2){
//     x0[i] = -.1 + delta*i;
//     x0[i+1] = .1 - delta*i;
//   }

//   for (State::data_iterator it=x0.begin(); it!=x0.end(); ++it)
//     cout << *it << " ";

//   cout << endl;

//   return 0;
// }


int main(int argc, char** argv){
  using namespace std;

  
  size_t num_state = atoi(argv[1]);
  size_t num_reps  = atoi(argv[2]);

  RHS ho(3, 0);

  // State x0(num_state, 2);
  // x0.col(0) = VarCol::LinSpaced(num_state, -.1, .1);
  // x0.col(1) = VarCol::LinSpaced(num_state, .1, -.1);

  Eigen::Matrix<double, Eigen::Dynamic, 2> xy(num_state, 2);
  xy.col(0) = Eigen::ArrayXd::LinSpaced(num_state, -.1, .1);
  xy.col(1) = Eigen::ArrayXd::LinSpaced(num_state, .1, -.1);
  
  State x0(xy);
    
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

  cout << mean << "+-" << stdev << ": ("
       << mean - stdev << ", "
       << mean + stdev << ")"  << endl;
  
  return steps;

}
