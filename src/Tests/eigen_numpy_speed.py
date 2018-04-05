import os, sys
home_dir = os.getenv("HOME")
py_core_root = home_dir + "/REPOS/INTEGRATOR"
sys.path.append(py_core_root) # add INTEGRATOR repo to pythonpath

import numpy as np
from time import clock
from element import MQuad
from rhs import RHS
from particle import Particle

transform = np.matrix('0 -1  0; 1 0 0; 0 0 1')
grad = 10.6
n_state = int(input("number of states  "))

qf = MQuad(5e-2, 8.6)

state = np.tile(np.repeat(0, 12), n_state)
def select(array, var_id):
    return [array[np.arange(var_id, len(array), 2)]]

p = Particle()
rhs = RHS(p, n_state, None)

rep_f = int(input("number of repetitions  "))
start = clock()
for i in range(rep_f):
    rhs(state, i*1e-6, qf)
    # x, = select(state, 0)
    # y, = select(state, 1)
    # vf = qf.BField(state)
    # vf = transform.dot(vf)

print("{} secs".format(clock()-start))
