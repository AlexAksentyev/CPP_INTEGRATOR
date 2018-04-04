
#ifndef TILT_H
#define TILT_H

#include <vector>
#include <iostream>
#include <Eigen/Dense>

namespace integrator {

using TiltAngleVector = std::vector<std::pair<char, double>>;
  class Tilt{
  
    TiltAngleVector tilt_angle_;
    Eigen::Vector3d axis(char name);

    // added for custom tilt
    double rad_angle_x_, rad_angle_s_;
    double tan_angle_x_, tan_angle_s_;
  
  public:
    Eigen::Affine3d transform;

    Tilt() : transform(Eigen::Matrix3d::Identity(3, 3)),
	     rad_angle_x_(0), rad_angle_s_(0),
	     tan_angle_x_(0), tan_angle_s_(0) {}

    double tan_angle_x(){return tan_angle_x_;}
    double tan_angle_s(){return tan_angle_s_;}

    void operator() (std::vector<std::pair<char, double>> axis_degangle,
		     bool append=false);

    void print();
    void clear();
  
  }; // class Tilt

  class Shift {
    double x_, y_;

  public:
    Shift(double x=0, double y=0) : x_(x), y_(y) {}

    double x(){return x_;}
    double y(){return y_;}
    
    Shift& operator+=(const Shift& rhs) {
      x_ += rhs.x_;
      y_ += rhs.y_;
      return *this;
    }

    void clear();
    
  }; // class Shift

  inline Shift operator+(Shift lhs, const Shift& rhs){
    lhs += rhs;
    return lhs;
  }
} // namespace integrator

#endif // TILT_H
