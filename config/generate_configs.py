import os, sys

home_dir = os.getenv("HOME")
py_integrator_root = home_dir + "/REPOS/INTEGRATOR"
sys.path.append(py_integrator_root) # add INTEGRATOR repo to pythonpath

from particle_log import StateList
from particle import Particle

out_dir = home_dir + "/REPOS/CPP_INTEGRATOR/config"
# generate initial state
state = StateList(Sz=1, dK=(-1e-4, 1e-4, 5), x=(-1e-3, 2e-4, 4))
state.write_to_file("state.conf", out_dir)
print("Generated state:")
print(state)
# generate particle data
particle = Particle()
particle.kinetic_energy += .5e-6*particle.kinetic_energy
particle.write_to_file("particle.conf", out_dir)
print("Generated particle:")
print(particle)
