#include "pointers_test.h"
#include <cstdlib>

using namespace std;

void Element::operator()() {
  cout << "host length: " << rhs_.host_length() << endl
       << "reference mass: " << rhs_.reference_mass() << endl;   
}

double RHS::host_length(){return host_.length_;}

double RHS::reference_mass(){return reference_.mass_;}


int main(int argc, char** argv){

  double mass = atof(argv[1]);
  double length = atof(argv[2]);
  
  Particle p(mass);
  Element e(length, p);

  e();

}
