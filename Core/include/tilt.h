
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
  
  public:
    Eigen::Affine3d transform;

    Tilt() : transform(Eigen::Matrix3d::Identity(3, 3)){}

    void operator() (std::vector<std::pair<char, double>> axis_degangle,
		     bool append=false);

    void print();
    void clear();
  
  };
} // namespace integrator

#endif // TILT_H
