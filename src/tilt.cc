

#include "tilt.h"


using namespace Eigen;
using namespace std;

Vector3d Tilt::axis(char name){
  switch(toupper(name)){
  case 'X': return Vector3d::UnitX();
  case 'Y': return Vector3d::UnitY();
  case 'Z': return Vector3d::UnitZ();
  case 'S': return Vector3d::UnitZ();
  default: cout << "Wrong axis!" << endl;
  }
}

void Tilt::operator() (vector<pair<char, double>> axis_degangle,
		       bool append){

  Affine3d result;
  if(append)
    result = transform;
  else
    result.setIdentity();

  Vector3d unit_axis;
  double radangle;
  for(vector<pair<char, double>>::iterator
	it = axis_degangle.begin();
      it != axis_degangle.end();
      ++it){
    unit_axis = axis((*it).first);
    radangle = M_PI/180*(*it).second;
    Affine3d t(AngleAxisd(radangle, unit_axis));
    result = t*result;
  }

  transform = result;
  tilt_angle_ = axis_degangle;
}

void Tilt::print(){

  if (tilt_angle_.size() == 0){
    cout << "No tilt" << endl;
    return;
  }
  
  for(TiltAngleVector::iterator it=tilt_angle_.begin();
      it!=tilt_angle_.end();
      ++it)
    cout << "axis: "<< it->first << " "
	 << "angle (deg): " << it->second
	 << endl;
}

void Tilt::clear() {
  tilt_angle_.clear();
  transform.setIdentity();
}
