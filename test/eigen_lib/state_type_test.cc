#include <iostream>
#include <eigen3/Eigen/Dense>

#include <math.h>


using namespace Eigen;
using namespace std;

static const int VAR_NUM = 3;

using state_type = Matrix<double, Dynamic, VAR_NUM>;

int main(int argc, char** argv)
{
  if (argc < 2){
    cout << "Insufficient arguments!\n";
    return 1;
  }
  
  int num = atoi(argv[1]);
  state_type state(num, VAR_NUM);

  for (int i=0; i<num; i++)
    state.row(i) << i+1, i+1, i+1;

  cout << "state: \n"
       << state
       << endl;
  

  return 0;
}
