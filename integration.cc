// implementation of the Tracker::track code piece
// TODO:
// * implement hdf5 file output the same way it's in python:
//   in the turns loop, every N turns. But first, implement 
//   .txt/.csv file output




#include <boost/numeric/odeint.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>

#include "right_hand_side.h"

using namespace std;
using namespace boost::numeric::odeint;

// the state type definition


// observer to log intermediate states from the integrator
// will have to implement hdf5 file output later
struct DataLog{
  vector<state_type>& system_state_;
  vector<double>& system_time_;
  
  DataLog(vector<state_type>& states, vector<double>& times)
  : system_state_(states), system_time_(times){}
  
  void operator() (const state_type &state_vec, double t){
    system_state_.push_back(state_vec);
    system_time_.push_back(t);
  }
};

int main(int argc, char* argv[]){

  double friction_coef = 0;
  double time_stop;

  cout << argc << endl;
  for(int i=0;i<argc;i++)
    cout << i << " " << argv[i] << endl;

  switch (argc){
  case 1:
    cout << "Too few arguments.\n";
    return 0;
  case 2:
    time_stop = atof(argv[1]);
    break;
  case 3:
    friction_coef = atof(argv[1]);
    time_stop = atof(argv[2]);
    break;
  default:
    cout << "Unknown case.\n";
    return 0;
  }

  cout << "friction: " << friction_coef << endl;
  cout << "time: " << time_stop << endl;

  
  RightHandSide rhs(friction_coef);
  vector<state_type> state;
  vector<double> time;
  DataLog log(state, time);
  state_type ini_state(2);
  ini_state[0] = 0.;
  ini_state[1] = 1.;

  double delta_time = .01;
  size_t num_steps = integrate(rhs,
			       ini_state, 0., time_stop, delta_time,
			       DataLog(state, time));

  ofstream data_file;
  data_file.open("boost_odeint_test.dat");
  int col_width = 10;
  data_file << fixed << setprecision(4);
  data_file << right
	    << setw(col_width) << "#time"
	    << setw(col_width) << "x"
	    << setw(col_width) << "v"
	    << endl;
  for (size_t i=0; i<num_steps; i++){
    data_file << right
	      << setw(col_width) << time[i]
	      << setw(col_width) << state[i][0]
	      << setw(col_width) << state[i][1]
	      << endl;
  }

  data_file.close();

  return (int)num_steps;
}
