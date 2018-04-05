
// TODO:
//      * append functionality in operator()

#include <Core/tilt.h>
#include <math.h>


using namespace Eigen;
using namespace std;
using namespace integrator::core;

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
		       bool append){ // append functionality is not properly implemented yet (assume False)

  Affine3d result;
  if(append){
    result = transform;
  }
  else {
    result.setIdentity();
    tilt_angle_.clear();
    // for custom tilt
    rad_angle_x_ = 0;
    rad_angle_s_ = 0;
    tan_angle_x_ = 0;
    tan_angle_s_ = 0;
  }

  Vector3d unit_axis;
  double radangle;
  for(vector<pair<char, double>>::iterator
	it = axis_degangle.begin();
      it != axis_degangle.end();
      ++it){
    unit_axis = axis(it->first);
    radangle = M_PI/180*it->second;
    Affine3d t(AngleAxisd(radangle, unit_axis));
    result = t*result;

    // custom tilt
    switch (toupper(it->first)){
    case 'X' : rad_angle_x_ += radangle;
    case 'S' : rad_angle_s_ += radangle;
    }

    tan_angle_x_ = std::tan(rad_angle_x_);
    tan_angle_s_ = std::tan(rad_angle_s_);
  }

  transform = result;
  tilt_angle_.insert(tilt_angle_.end(), axis_degangle.begin(), axis_degangle.end());
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

void Shift::clear() {
  x_ = 0; y_ = 0;
}
