#include <iostream>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

#include <math.h>


using namespace Eigen;
using namespace std;

Eigen::Vector3d axis(char name){
  switch(toupper(name)){
  case 'X': return Eigen::Vector3d::UnitX();
  case 'Y': return Eigen::Vector3d::UnitY();
  case 'Z': return Eigen::Vector3d::UnitZ();
  default: cout << "Wrong axis!" << endl;
  }
}

int main(int argc, char** argv)
{
  if (argc < 4){
    cout << "Insufficient arguments! (need 3)";
    return 1;
  }

  int num_state = atoi(argv[1]);
  double angle = atof(argv[2])*M_PI/180;
  Vector3d u = axis(*argv[3]);

  Vector3d field(1,0,0);
  Matrix<double, 3, Dynamic> vec_field;
  vec_field = field.replicate(1, num_state);
  
  Affine3d t(AngleAxisd(angle, u));

  cout << "f : \n"
       << field
       << endl;

  cout << "M : \n"
       << t.rotation()
       <<endl;

  t *= t;
  
  cout << "M*M : \n"
       << t.rotation()
       << endl;

    cout << "M*M*f : \n"
       << t*field
       << endl;

  
  return 0;
}
