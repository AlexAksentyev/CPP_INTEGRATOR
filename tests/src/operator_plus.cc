
#include "operator_plus.h"
#include <iostream>

using namespace std;


int main () {

  Lattice first(0);
  first = {1, 6, 7, 8};
  Lattice second(1);
  second = {-4, -5, -2};

  second += first;

  cout << second << endl;
  
  return 0;
}
