#  SALOME Utils : general SALOME's definitions and tools
#
#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#
#
#  File   : SALOME_utilities.py
#  Module : SALOME

import SALOME_Trace
GLogger = SALOME_Trace.SALOME_Trace()

def MYTRACE ():
    if __debug__:
        str = "- Trace "
        GLogger.putMessage(str + "  : ")
        

def REPERE():
    if __debug__:
        GLogger.putMessage("   --------------  \n")


def BEGIN_OF(msg):
    if __debug__:
        REPERE(); MYTRACE();
        GLogger.putMessage("Begin of : "+ str(msg) + "\n")
        REPERE();


def END_OF(msg):
    if __debug__:
        REPERE(); MYTRACE();
        GLogger.putMessage("Normale end of : "+ str(msg) + "\n")
        REPERE();

def MESSAGE(msg):
    if __debug__:
        MYTRACE()
        GLogger.putMessage(str(msg) + "\n")

def SCRUTE(var_name, var_value):
    MYTRACE();
    GLogger.putMessage(var_name + " = " + str(var_value) + "\n")

   
