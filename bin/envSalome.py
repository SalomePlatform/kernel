#!/usr/bin/env python
import os
import sys

kernel_root=os.getenv("KERNEL_ROOT_DIR")
sys.path[:0]=[kernel_root+"/bin/salome"]

import runSalome

argv=sys.argv[1:]
sys.argv=[]

args, modules_list, modules_root_dir = runSalome.get_config()
runSalome.set_env(args, modules_list, modules_root_dir)

os.execvp(argv[0],argv)
