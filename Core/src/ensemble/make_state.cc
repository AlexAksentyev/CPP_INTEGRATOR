
#include <string>
#include <stdlib.h>

#include "right_hand_side.h" // provides VMAP, VAR_NUM, State

using namespace std;
using namespace rhs;
using namespace Eigen;

State main (int argc, char** argv) {
  // argument format:
  // var_name low_bound high_bound num_of_points

  // vector<string> var_name_vec
  // for(int var_name=1; var_name<argc-4; var_name+=4){    
  // }

  

  State test_state (3, VAR_NUM);
  state.setZero();
  state.col(VMAP.left.at(argv[1])) = VectorXd::Ones(atof(argv[2]));

  return state;
}


