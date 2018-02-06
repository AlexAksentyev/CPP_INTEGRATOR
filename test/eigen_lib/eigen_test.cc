#include <iostream>
#include <eigen3/Eigen/Dense>

#include <math.h>


using namespace Eigen;
using namespace std;

using state_type = Matrix<double, Dynamic, 3>;
template<typename T>
using Vector = Matrix<T, Dynamic, 1>;
template<typename T>
using fn_ptr = T (*)(T);

void modify(state_type& state){
  state.col(1) += 1*VectorXd::Ones(state.rows());
}

template<typename T>
class variable_col {
public:
  Vector<T> data;

  variable_col(Vector<T> given, fn_ptr<T> fn){
    int num = given.rows();
    data.setZero(num);    
    for (int i=0; i<num; i++)
      data[i] = fn(given[i]);
  }
  
};

template<typename T>
T add1(T x){return x+1;}

int main(int argc, char** argv)
{

  int num_states = atoi(argv[1]);
  int which = atoi(argv[2]);

  state_type state(num_states, 3);

  for(int i=0; i<num_states; i++)
    state.row(i) << 1, 2, 3;

  fn_ptr<double> add1_ptr = &add1;
  variable_col<double> v(state.col(which), add1_ptr);

  cout << v.data << endl;
  
}
