#!/usr/bin/env python
import os, string, sys, re

from killSalomeWithPort import killMyPort, getPiDict

def killAllPorts():
    user = os.getenv('USER')
    filedict = "^%s$"%(getPiDict('(\d*)',full=False))
    fnamere = re.compile(filedict)
    for file in os.listdir(os.getenv("HOME")):
        mo = re.match(fnamere,file)
        if mo and len(mo.groups()):
            killMyPort(mo.groups()[0])
        pass

if __name__ == "__main__":
    killAllPorts()
