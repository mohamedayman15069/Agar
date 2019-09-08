"""
File: __init__.py
Date: 9/7/19 
Author: Jon Deaton (jonpauldeaton@gmail.com)
"""

import numpy as np

null_action = (np.zeros(2), 0)

default_config = {
            'ticks_per_step':  4,
            'num_frames':      1,
            'arena_size':      1000,
            'num_pellets':     1000,
            'num_viruses':     25,
            'num_bots':        25,
            'pellet_regen':    True,
            'grid_size':       128,
            'observe_cells':   True,
            'observe_others':  True,
            'observe_viruses': True,
            'observe_pellets': True
        }

