#  SALOME Logger : CORBA server managing trace output
#
#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#
#
#  File   : SALOME_Trace.py
#  Module : SALOME

import sys
import CosNaming
from omniORB import CORBA
import SALOME_Logger
import time
import os

trace="local"
if (os.environ.has_key("SALOME_trace")):
    if (os.environ["SALOME_trace"] == "with_logger"):
        trace="logger"

class SALOME_Trace :
    def __init__(self):
        self.m_pInterfaceLogger = None
        if trace=="logger":
            ok = 0
            steps = 40
            while steps > 0 and ok == 0:

              try:
                orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
                theObj = orb.resolve_initial_references("NameService")
                inc = theObj._narrow(CosNaming.NamingContext)
                name = [CosNaming.NameComponent("Logger","")]
                obj = inc.resolve(name);

                self.m_pInterfaceLogger = obj._narrow(SALOME_Logger.Logger)

                if not self.m_pInterfaceLogger is None:
                  ok = 1

              except CosNaming.NamingContext.NotFound, e :
                    if steps == 1: print "Caught exception: Naming Service can't found Logger"
              except CORBA.COMM_FAILURE, e:
                    if steps == 1: print "Caught CORBA::SystemException CommFailure"
              except CORBA.SystemException, e:
                    if steps == 1: print "Caught CORBA::SystemException."
              except CORBA.Exception, e:
                    if steps == 1: print "Caught CORBA::Exception."
              except Exception, e:
                    if steps == 1: print "Caught unknown exception."

              time.sleep(0.25)
              steps = steps - 1
	  
	  
    def putMessage ( self, LogMsg ) :
        
        if (CORBA.is_nil(self.m_pInterfaceLogger)):
            print  LogMsg;
        else:
            self.m_pInterfaceLogger.putMessage (LogMsg) 
   
