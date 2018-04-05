#include <iostream>
#include <Core/state.h>



int main () {

  using namespace std;
  using namespace core;

  string home_dir = getenv("HOME");
  string root_dir = home_dir + "/REPOS/CPP_INTEGRATOR";
  string config_dir = root_dir+"/config";

  
  // cout << "Reading particle config" << endl;
  // Particle p = Particle::from_config(config_dir + "/particle.conf");

  State s = State::from_config(config_dir + "/state.conf");

  VariableCol x = s.var("x");
  VariableCol y = s.var("y");
  VariableCol z = x+y;

  for (VariableCol::iterator it=z.begin(); it!=z.end(); ++it)
    cout << *it << endl;
  
  cout << "good" << endl;

  return 0;

}
