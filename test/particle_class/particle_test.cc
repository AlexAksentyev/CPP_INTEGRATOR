#include <iostream>
#include <stdlib.h>
#include "particle.h"

using namespace std;

int main(int argc, char** argv){
  if (argc == 4){
    double mass = atof(argv[1]);
    double gamma = atof(argv[2]);
    double G = atof(argv[3]);

      Particle p1(mass, gamma, G);
      std::cout << "p1:" << std::endl;
      p1.print();
  }
  
  Particle p0;
  p0.print();
 
  double gamma, beta;
  p0.GammaBeta(gamma, beta, 1e-2);
  cout << "GammaBeta:" << endl;
  cout << gamma << "\t" << beta << endl;
  double Pc = p0.Pc(1e-2);
  double freq = p0.revolution_freq(100);
  cout << "freq: " << freq << endl
       << "Pc: " << Pc << endl;

  return 0;
};
