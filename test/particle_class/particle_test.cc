#include <iostream>
#include <stdlib.h>
#include "particle.h"
#include <iomanip>
#include <Eigen/Dense>


using namespace std;

int main(int argc, char** argv){
  // argument check
  if (argc < 2){
    cout << "Expecting a dK argument" << endl;
    return 1;
  }

  variable_col dK(6);
  for (int i=0; i<6; i++)
    dK[i] = atof(argv[1])*i;  
  
  Particle p0;

  variable_col vec = p0.Pc(dK);
  cout << "Vector:\n" << vec << endl;
  
 
  return 0;
};
