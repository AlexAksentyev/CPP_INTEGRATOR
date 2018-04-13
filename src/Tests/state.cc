#include <iostream>
#include <Tests/state.h>


int main() {

  using namespace integrator::core;
  using namespace std;
  
  State::Vector6d d; d << .1, .1, 0, 0, 0, 0;
  State::Vector3d s; s << 0, 0, 1;

  State state(d, s);
  
  cout << state << endl;

  state(5) = 1e-4;
  cout << state(1) << "," << state(5) << endl;
  state[1] = -1;
  cout << state[0] << "," << state[1] << "," << state[2] << endl;
  
}
