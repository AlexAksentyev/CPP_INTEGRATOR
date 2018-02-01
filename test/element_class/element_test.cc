

#include <iostream>
#include "element.h"
#include <string>

#include <eigen3/Eigen/Dense>

using namespace std;

int main(int argc, char** argv){

  if (argc < 2){
    cout << "Insufficient arguments!"
	 << "(need number of particles)"
	 << endl;
    return 1;
  }

  int num_states = atoi(argv[1]);
  
  Element e(0, 25);

  state_type state(num_states, VAR_NUM);

  for(int i=0; i<num_states; i++)
    for(int j=0; j<VAR_NUM; j++)
      state(i,j) = i+1;


  cout << "state: \n"
       << state
       << endl;
  
  e.vectorize_fields(state);

  e.print_fields();

  e.print_vectorized_fields();

  return 0;
}



