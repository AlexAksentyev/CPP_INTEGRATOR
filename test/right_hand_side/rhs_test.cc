#include <iostream>
#include "particle.h"
#include "right_hand_side.h"



int main (int argc, char** argv){


  Particle_ptr p0(new Particle());
  RightHandSide rhs(p0);

  rhs.particle_->print();
  
}
