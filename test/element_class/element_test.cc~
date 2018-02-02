

#include <iostream>
#include "element.h"
#include <string>

#include <eigen3/Eigen/Dense>


int main(int argc, char** argv){
  using namespace std;

  if (argc < 3){
    cout << "Insufficient arguments!"
	 << "(give pairs direction-angle)"
	 << endl;
    return 1;
  }

  if(remainder(argc-1, 2) != 0){
    cout << "Unfinished pair!" << endl;
    return 2;
  }
  int num_pairs = (argc-1)/2;
  vector<pair<char, double>> ax_an;
  for(int i=1; i<argc; i+=2){
    ax_an.push_back(make_pair(*argv[i], atof(argv[i+1])));
  }

  int num_states = 2;
  
  Element e(0, 25);

  state_type state(num_states, VAR_NUM);

  for(int i=0; i<num_states; i++)
    for(int j=0; j<VAR_NUM; j++)
      state(i,j) = i+1;


  cout << "state: \n"
       << state
       << endl;
  
  e.vectorize_fields(state);

  cout << "Tilt matrix : \n"
       << e.tilt_.transform_.rotation() << endl;

  cout << "M*(0,0,1) : \n"
       << e.tilt_.transform_*Eigen::Vector3d::UnitZ()
       << endl;

  e.tilt_(ax_an);

  cout << "Tilt matrix after tilt : \n"
       << e.tilt_.transform_.rotation() << endl;

  cout << "M*(0,0,1) : \n"
       << e.tilt_.transform_*Eigen::Vector3d::UnitZ()
       << endl;
  
  return 0;
}



