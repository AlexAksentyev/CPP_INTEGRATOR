#ifndef BNL_H
#define BNL_H

#include <Core/lattice.h>
#include <Core/particle.h>
#include <string>

namespace lattice {

  integrator::Lattice compose(integrator::Particle& particle, std::string name);
  
} // namespace lattice


#endif // BNL_H
