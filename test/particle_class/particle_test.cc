#include <iostream>
#include <stdlib.h>
#include "particle.h"
#include <iomanip>

using namespace std;

int main(int argc, char** argv){
  // argument check
  if (argc < 2){
    cout << "Expecting a dK argument" << endl;
    return 1;
  }

  double dK = atof(argv[1]);
  
  Particle p0;
  
  cout << "dK: " << dK << endl;
  
  double gamma = p0.gamma(dK);
  double beta = p0.beta(dK);
  cout << "gamma, beta:" << endl;
  cout << gamma << "\t" << beta << endl;
  double Pc = p0.Pc(dK);
  p0.set_kinetic_energy(p0.kinetic_energy(dK));
  double freq = p0.revolution_freq(100);
  cout << "freq: " << setprecision(8) << freq << endl
       << "Pc: " << Pc << endl;

  return 0;
};
