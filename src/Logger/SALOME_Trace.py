#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#  SALOME Logger : CORBA server managing trace output
#  File   : SALOME_Trace.py
#  Module : SALOME
#
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
              except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
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
