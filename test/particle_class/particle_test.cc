#include <iostream>
#include <stdlib.h>
#include "particle.h"

int main(int argc, char** argv){
  if (argc > 1){
    double mass = atof(argv[1]);
    double gamma = atof(argv[2]);
    double G = atof(argv[3]);

      Particle p1(mass, gamma, G);
      std::cout << "p1:" << std::endl;
      p1.print();
  }
  
  Particle p0;
  std::cout << "p0:" << std::endl;
  std::cout << p0.mass0_ << std::endl;
  p0.print();

  return 0;
};
