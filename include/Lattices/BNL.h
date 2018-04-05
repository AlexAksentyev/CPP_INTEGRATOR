#ifndef BNL_H
#define BNL_H

#include <Core/lattice.h>
#include <Core/particle.h>
#include <string>

namespace integrator{
  namespace lattice {

    integrator::core::Lattice compose(integrator::core::Particle& particle, std::string name);
  
  } // namespace lattice
} //namespace integrator

#endif // BNL_H
