#!/usr/bin/env python
import os, string, sys

from killSalomeWithPort import killMyPort

def killAllPorts():
    user = os.getenv('USER')
    for file in os.listdir(os.getenv("HOME")):
        l = string.split(file, "_")
        if len(l) >= 4:
            if file[:len(user)] == user:
                if l[len(l)-2] == "SALOME" and l[len(l)-1] == "pidict":
                    killMyPort(l[len(l)-3])
        pass

if __name__ == "__main__":
    killAllPorts()
