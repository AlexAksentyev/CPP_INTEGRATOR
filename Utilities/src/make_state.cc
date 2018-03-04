
#include "right_hand_side.h"

#include <iostream>
#include <iomanip>
#include <Eigen/Dense>
#include <stdlib.h>
#include <string>
#include <map>
#include <boost/tuple/tuple.hpp>

using namespace integrator::rhs;
using namespace std;
using namespace Eigen;

int main(int argc, char** argv){

  size_t num_of_states=1, group_num;
  string var_name;
  double low_bound, up_bound;
  using BoundTuple = boost::tuple<double, double, size_t>;
  map<string, BoundTuple> col_map;
  for (size_t i=1; i<=argc-4; i+=4){
    if (strncmp(argv[i+3], "all", 3) == 0)
      group_num = 1;
    else
      group_num = atoi(argv[i+3]);
    var_name = argv[i];
    low_bound = atof(argv[i+1]);
    up_bound = atof(argv[i+2]);
    cout << "group number: " << group_num << endl;
    num_of_states *= group_num;
    col_map.insert(pair<string, BoundTuple>(var_name, BoundTuple(low_bound, up_bound, group_num)));
  }

  cout << "number of states: "<< num_of_states << endl;

  State state(num_of_states, VAR_NUM);
  state.setZero();
  // NOT WORKING 
  for(map<string, BoundTuple>::iterator it=col_map.begin();
      it!=col_map.end();
      ++it){
    var_name = it->first;
    low_bound = it->second.get<0>();
    up_bound = it->second.get<1>();
    group_num = it->second.get<2>();
    state.col(VMAP.left.at(var_name)) = VectorXd::LinSpaced(group_num, low_bound, up_bound);
  }

  cout << fixed << setprecision(2) << state << endl;

  return 0;
}
