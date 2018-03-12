
#include <iostream>
#include <stdlib.h>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <boost/numeric/odeint.hpp>

#include <time.h>

#include <Core/right_hand_side.h>
#include <Core/particle.h>
#include <Core/elements/quadrupole.h>

using VecField = Eigen::Matrix<double, 3, Eigen::Dynamic>;
using State = Eigen::Matrix<double, Eigen::Dynamic, 12>;

int main(int argc, char** argv){

  using namespace Eigen;
  using namespace std;
  using namespace integrator;
  using namespace integrator::element;
  using namespace integrator::rhs;
  using namespace boost::numeric::odeint;
  
  runge_kutta_dopri5<State, double,
		     State, double,
		     vector_space_algebra> stepper;

  Particle p;
  MQuad qf(p, 5e-2, 8.6);
  RightHandSide rhs(p, qf);

  Affine3d transform(AngleAxisd(M_PI/2, Vector3d::UnitZ()));
  double grad = 10.6;
  int n_state = atoi(argv[1]);
  State state(MatrixXd::Zero(n_state, 12)), deriv(MatrixXd::Zero(n_state, 12));
  state.col(0) = VectorXd::Ones(n_state);
  state.col(1) = 2*VectorXd::Ones(n_state);

  VecField vf(MatrixXd::Zero(3, n_state));
  qf.vectorize_fields(state);
  
  int rep_f = atoi(argv[2]);
  clock_t t;
  t = clock();
  for (int i=0; i < rep_f; i++){
    rhs(state, deriv, i*1e-6);
    // vf = qf.BField(state);
    // vf = transform*vf;
  }
  t = clock() - t;
  printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);


}
