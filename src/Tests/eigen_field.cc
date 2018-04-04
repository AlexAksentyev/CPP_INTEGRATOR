#include <vector>
#include <iostream>
#include <Eigen/Dense>



int main (int argc, char** argv) {

  using namespace std;

  using Field_t = Eigen::Matrix<double, 3, Eigen::Dynamic>;

  Field_t F0(3, 6);
  Eigen::ArrayXd I = Eigen::ArrayXd::Ones(6);
  F0.row(0) = I;
  F0.row(1) = 2*I;
  F0.row(2) = 3*I;

  cout << F0 << endl<<endl;

  Eigen::Vector3d dF(1,1,1);
  Field_t F1 = F0 + dF.replicate(1, 6);

  cout << F1 << endl << endl;

  cout << dF << endl << endl;

  dF << 1,2,3;

  cout << dF << endl << endl;


  // vector<double> a(3, 5);
  // vector<double> b(3, -5);

  // for (vector<double>::iterator it=a.begin(); it!=a.end(); ++it){
  //   cout << *it << " ";
  // }
  // cout << endl << endl;
  // a.clear();
  // a.insert(a.end(), b.begin(), b.end());

  // for (vector<double>::iterator it=a.begin(); it!=a.end(); ++it){
  //   cout << *it << " ";
  // }
  // cout << endl << endl;

}
