

#include <iostream>
#include "particle.h"
#include "quadrupole.h"
#include "drift_space.h"
#include "dipole.h"
#include <string>

#include <eigen3/Eigen/Dense>


int main(int argc, char** argv){
  using namespace std;
  // checking argument presence
  if (argc < 3){
    cout << "Insufficient arguments!"
	 << "(give pairs direction-angle)"
	 << endl;
    return 1;
  }

  // checking argument correctness
  if(remainder(argc-1, 2) != 0){
    cout << "Unfinished pair!" << endl;
    return 2;
  }
  int num_pairs = (argc-1)/2;
  vector<pair<char, double>> ax_an;
  for(int i=1; i<argc; i+=2){
    ax_an.push_back(make_pair(*argv[i], atof(argv[i+1])));
  }

  // creating the particle
  Particle_ptr deu(new Particle());

  // creating the state ensemble
  int num_states = 2;
  state_type state(num_states, VAR_NUM);
  for(int i=0; i<num_states; i++)
    for(int j=0; j<VAR_NUM; j++)
      state(i,j) = j;

  MSolenoid element(205e-3, 3.4);
  element.vectorize_fields(state);

  element.print();

  cout << "element B-field : \n"
       << element.BField(state)
       << endl;

  element.tilt_(ax_an);
  element.vectorize_fields(state);

  cout << "tilted field : \n"
       << element.BField(state)
       << endl;

  return 0;
}



