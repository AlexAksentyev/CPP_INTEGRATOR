#include "Core/elements/dipole.h"

using namespace integrator::element;

// horizontally bending magnetic dipole
// B = (0, By, 0)
MDipole::MDipole(Particle& reference_particle,
		 double length, double B_vert,
		 std::string name)
  : TiltableElement(reference_particle, B_vert*CLIGHT/reference_particle.Pc()*1e-6, length, name){

  B_field_base_(1) = B_vert;
}

MSolenoid::MSolenoid(Particle& particle,
		     double length, double B_longitudinal,
		     std::string name)
  : Element(particle, 0, length, name){
  B_field_base_(2) = B_longitudinal;
}
