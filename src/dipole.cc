#include "dipole.h"

// horizontally bending magnetic dipole
// B = (0, By, 0)
MDipole::MDipole(double length, Particle_ptr reference_particle,
		 double B_vert, std::string name)
  : Element(B_vert*CLIGHT/reference_particle->Pc()*1e-6, length, name){

  B_field_base_(1) = B_vert;
}

MSolenoid::MSolenoid(double length, double B_longitudinal, std::string name)
  : Element(0, length, name){
  B_field_base_(2) = B_longitudinal;
}
