#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <Eigen/Dense>
#include <string>
#include <vector>
#include <math.h>

namespace integrator {
  const double CLIGHT = 2.99792458e8; // m/s
  const double EZERO = 1.602176462e-19; // Coulomb

  using VariableCol = std::vector<double>;//Eigen::ArrayXd;

  VariableCol& operator+=(VariableCol& lhs, const VariableCol& rhs){
    size_t n_lhs = lhs.size();
    size_t n_rhs = rhs.size();
    if (n_lhs != n_rhs)
      std::cout << "LHS size != RHS size" << std::endl;

    for (size_t i=0; i< n_lhs; i++)
      lhs[i] += rhs[i];

    return lhs;
  }
  inline VariableCol operator+(VariableCol lhs, const VariableCol& rhs){
    lhs += rhs;
    return lhs;
  }

  VariableCol& operator-=(VariableCol& lhs, const VariableCol& rhs){
    size_t n_lhs = lhs.size();
    size_t n_rhs = rhs.size();
    if (n_lhs != n_rhs)
      std::cout << "LHS size != RHS size" << std::endl;

    for (size_t i=0; i< n_lhs; i++)
      lhs[i] -= rhs[i];

    return lhs;
  }
  inline VariableCol operator-(VariableCol lhs, const VariableCol& rhs){
    lhs -= rhs;
    return lhs;
  }

  VariableCol& operator*=(VariableCol& vec, double factor){
    for (VariableCol::iterator it=vec.begin();
	 it!=vec.end();
	 ++it)
      *it *= factor;

    return vec;
  }

  inline VariableCol operator*(double factor, const VariableCol& vec){
    VariableCol res(vec);
    res *= factor;
    return res;
  }
  inline VariableCol operator*(const VariableCol& vec, double factor){
    VariableCol res(vec);
    res *= factor;
    return res;
  }

  VariableCol& operator/=(VariableCol& vec, double factor){
    for (VariableCol::iterator it=vec.begin();
	 it!=vec.end();
	 ++it)
      *it /= factor;

    return vec;
  }

  inline VariableCol operator/(const VariableCol& vec, double factor){
    VariableCol res(vec);
    res /= factor;
    return res;
  }

  VariableCol& operator/=(VariableCol& lhs, const VariableCol& rhs){
    size_t n_lhs = lhs.size();
    size_t n_rhs = rhs.size();
    if (n_lhs != n_rhs)
      std::cout << "LHS size != RHS size" << std::endl;

    for (size_t i=0; i< n_lhs; i++)
      lhs[i] /= rhs[i];

    return lhs;
  }

  inline VariableCol operator/(VariableCol lhs, const VariableCol& rhs){
    lhs /= rhs;
    return lhs;
  }

  VariableCol& operator*=(VariableCol& lhs, const VariableCol& rhs){
    size_t n_lhs = lhs.size();
    size_t n_rhs = rhs.size();
    if (n_lhs != n_rhs)
      std::cout << "LHS size != RHS size" << std::endl;

    for (size_t i=0; i< n_lhs; i++)
      lhs[i] *= rhs[i];

    return lhs;
  }

  inline VariableCol operator*(VariableCol lhs, const VariableCol& rhs){
    lhs *= rhs;
    return lhs;
  }

  VariableCol sqrt(VariableCol vec){
    for(VariableCol::iterator it=vec.begin();
	it!=vec.end();
	++it)
      *it = std::sqrt(*it);

    return vec;
  }

  std::ostream& operator<<(std::ostream& out_stream, VariableCol& vec){
    for (VariableCol::iterator it=vec.begin();
	 it!=vec.end();
	 ++it)
      out_stream << *it << std::endl;
    return out_stream;
  }

  class Particle{
    double gamma_;
    double kin_nrg_0_;

    double mass0_;
    double G_;
    double mass0_kg_;


  public:  
    Particle(double mass0=1876.5592,
	     double gamma=1.14394,
	     double G=-.142987);
  

    double mass0();
    double mass0_kg();
    double G();

    double kinetic_energy(double relative_delta_energy=0);
    VariableCol kinetic_energy(VariableCol relative_delta_energy); // no default because dynamic allocation
    void set_kinetic_energy(double value);
    double gamma(double relative_delta_energy=0);
    VariableCol gamma(VariableCol relative_delta_energy);
    void set_gamma(double value);
    double beta(double relative_delta_energy=0);
    VariableCol beta(VariableCol relative_delta_energy);
    double Pc(double relative_delta_energy=0);
    VariableCol Pc(VariableCol relative_delta_energy);
  
    double revolution_freq(double lattice_length);

    void print(){std::cout << mass0_ << " " << gamma_ << " " << G_ << std::endl;}

    static Particle from_config(const std::string & path);
  };  // class particle

  // Particle read_particle_csv(const std::string & path);
} // namespace integrator

#endif
