import os, sys

home_dir = os.getenv("HOME")
py_integrator_root = home_dir + "/REPOS/INTEGRATOR"
sys.path.append(py_integrator_root) # add INTEGRATOR repo to pythonpath

from particle_log import StateList
from particle import Particle


# generate initial state
state = StateList(Sz=1, dK=(-1e-4, 1e-4, 5), x=(-1e-3, 2e-4, 4))
state.write_to_file("state.conf", ".")

deu = Particle()
deu.write_to_file("particle.conf", ".")
