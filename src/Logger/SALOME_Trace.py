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

class SALOME_Trace :
    def __init__(self):
        self.m_pInterfaceLogger = None
        try:
            orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
            theObj = orb.resolve_initial_references("NameService")
            inc = theObj._narrow(CosNaming.NamingContext)
            name = [CosNaming.NameComponent("Logger","")]
            obj = inc.resolve(name);
            
            self.m_pInterfaceLogger = obj._narrow(SALOME_Logger.Logger)

        except CosNaming.NamingContext.NotFound, e :
                print "Caught exception: Naming Service can't found Logger"
        except CORBA.COMM_FAILURE, e:
                print "Caught CORBA::SystemException CommFailure"
        except CORBA.SystemException, e:
                print "Caught CORBA::SystemException."
        except CORBA.Exception, e:
                print "Caught CORBA::Exception."
        except Exception, e:
                print "Caught unknown exception."
                

    def putMessage ( self, LogMsg ) :
        
        if (CORBA.is_nil(self.m_pInterfaceLogger)):
            print  LogMsg;
        else:
            self.m_pInterfaceLogger.putMessage (LogMsg) 
   
