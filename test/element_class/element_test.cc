

#include <iostream>
#include "element.h"
#include <string>

using namespace std;

int main(int argc, char** argv){

  if (argc < 4){
    cout << "Insufficient arguments!" << endl;
    return 1;
  }

  double crv = atof(argv[1]);
  double len = atof(argv[2]);
  std::string nam = argv[3];
  
  Element e(crv, len, nam);

  cout << e.curve() << endl
       << e.length() << endl
       << e.name() << endl;


  e.print_fields();

  return 0;
}



