#include "Core/particle.h"

#include <vector>
#include <fstream>

using namespace std;
// using namespace Eigen;
using namespace integrator;

Particle::Particle(double mass0, double gamma, double G)
  : mass0_(mass0), gamma_(gamma), G_(G){

  mass0_kg_ = mass0_*EZERO*1e6/(CLIGHT*CLIGHT);
  kin_nrg_0_ = mass0_*(gamma_ - 1);
}

double Particle::mass0(){return mass0_;}
double Particle::mass0_kg(){return mass0_kg_;}
double Particle::G(){return G_;}

double Particle::kinetic_energy(double relative_dK){
  return kin_nrg_0_*(1 + relative_dK);
}

VariableCol Particle::kinetic_energy(VariableCol relative_dK){
  VariableCol I = VariableCol(relative_dK.size(), 1);//VariableCol::Ones(relative_dK.rows());
  return kin_nrg_0_*(I + relative_dK);
}

void Particle::set_kinetic_energy(double value){
  // sanity check
  if (value < 0){
    cout << "Negative energy!" << endl;
    return;
  }

  kin_nrg_0_ = value;
  gamma_ = 1 + value/mass0_;
}

double Particle::gamma (double relative_dK){
  double kin_nrg = kinetic_energy(relative_dK);
  return kin_nrg/mass0_ + 1;
}

VariableCol Particle::gamma (VariableCol relative_dK){
  VariableCol I = VariableCol(relative_dK.size(), 1); //VariableCol::Ones(relative_dK.rows());
  VariableCol kin_nrg = kinetic_energy(relative_dK);
  return kin_nrg/mass0_ + I;
}

void Particle::set_gamma(double value){
  // sanity check
  if(value < 0){
    cout << "Negative gamma!" << endl;
    return;
  }

  gamma_ = value;
  kin_nrg_0_ = mass0_*(value - 1);
}

double Particle::beta(double relative_dK){
  double g = gamma(relative_dK);
  return std::sqrt(g*g - 1)/g;
}

VariableCol Particle::beta(VariableCol relative_dK){
  VariableCol I = VariableCol(relative_dK.size(), 1); //VariableCol::Ones(relative_dK.rows());
  VariableCol g = gamma(relative_dK);
  return sqrt(g*g - I)/g;
}

double Particle::Pc(double relative_dK){
  double kin_nrg = kinetic_energy(relative_dK);
  double tot_nrg = mass0_ + kin_nrg;
  return std::sqrt(tot_nrg*tot_nrg - mass0_*mass0_);
}

VariableCol Particle::Pc(VariableCol relative_dK){
  VariableCol I = VariableCol(relative_dK.size(), 1); //VariableCol::Ones(relative_dK.rows());
  VariableCol kin_nrg = kinetic_energy(relative_dK);
  VariableCol tot_nrg = mass0_*I + kin_nrg;
  return sqrt(tot_nrg*tot_nrg - mass0_*mass0_*I);
}

double Particle::revolution_freq(double lattice_length){
  double v = beta()*CLIGHT;
  return v/lattice_length;
}

Particle Particle::from_config(const std::string & path){
  string header, data;
  vector<double> values;
  ifstream particle_file;
  particle_file.open(path);
  getline(particle_file, header);
  cout << "Config header: " << header << endl; // Should be Mass, KinEn, gamma, G
  getline(particle_file, data);
  stringstream lineStream(data);
  string cell;
  while (getline(lineStream, cell, ',')) {
    values.push_back(stod(cell));
  }
  particle_file.close();
  return Particle(values[0], values[2], values[3]); // take Mass, gamma, G
}

// Particle integrator::read_particle_csv(const string & path){
//   cout << "Particle read_particle" << endl;
//   string header, data;
//   vector<double> values;
//   ifstream particle_file;
//   particle_file.open(path);
//   getline(particle_file, header);
//   cout << header << endl;
//   getline(particle_file, data);
//   stringstream lineStream(data);
//   string cell;
//   while (getline(lineStream, cell, ',')) {
//     values.push_back(stod(cell));
//   }
//   return Particle(values[0], values[1], values[2]);
// }
