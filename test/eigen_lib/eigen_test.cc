#include <iostream>
#include <eigen3/Eigen/Dense>

#include <math.h>


using namespace Eigen;
using namespace std;

int main(int argc, char** argv)
{
  if (argc < 3){
    cout << "Insufficient arguments\n";
    return 1;
  }

  double angle = atof(argv[1])*M_PI/180.;
  double cosA = cos(angle);
  double sinA = sin(angle);

  int num_states = atoi(argv[2]);
  
  Matrix3d tilt_M(3, 3);
  tilt_M <<
    cosA, 0, -sinA,
    0,    1, 0,
    sinA, 0, cosA;

   cout << "here\n\n";
  
   Vector3d field(1, 0, 0);
   Matrix<double, 3, Dynamic> vec_field(3, num_states);
   vec_field = field.replicate(1, num_states);

  cout << "field: \n";
  cout << vec_field  << endl;

  cout << "tilt matrix: \n";
  cout << tilt_M << endl << endl;

  cout << "tilted field: \n";
  cout << tilt_M * vec_field << endl << endl;

  cout << "tilt matrix third column : \n"
       << tilt_M.col(2)
       << endl;

  cout << "tilt matrix first row : \n"
       << tilt_M.row(0)
       << endl;
}
