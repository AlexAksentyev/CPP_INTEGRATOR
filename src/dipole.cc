#include "dipole.h"

// horizontally bending magnetic dipole
// B = (0, By, 0)
MDipole::MDipole(double length, Particle_ptr reference_particle,
		 double By, std::string name)
  : Element(By*CLIGHT/reference_particle->Pc()*1e-6, length, name){

  B_field_base_(1) = By;
}
