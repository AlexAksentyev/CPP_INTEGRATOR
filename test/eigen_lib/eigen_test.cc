#include <iostream>
#include <eigen3/Eigen/Dense>

#include <math.h>


using namespace Eigen;
using namespace std;

using state_type = Matrix<double, Dynamic, 3>;

void modify(state_type& state){
  state.col(1) += 1*VectorXd::Ones(state.rows());
}


int main(int argc, char** argv)
{
  if (argc < 2)
    return 1;

  int num_states = atoi(argv[1]);

  state_type state(num_states, 3);

  for(int i=0; i<num_states; i++)
    state.row(i) << 1, 2, 3;

  cout << "state : \n" << state << endl << endl;

  modify(state);
  
  cout << "state : \n" << state << endl << endl;
  
}
