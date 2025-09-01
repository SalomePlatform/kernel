#! /usr/bin/env python3
# Copyright (C) 2021-2025  CEA, EDF
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
from . import setenv_impl
from . import runSalomeCommon
import os
import logging
logger = logging.getLogger()

class NoSessionServer(runSalomeCommon.CommonSessionServer):
    def __init__(self,args,modules_list,modules_root_dir):
        super().__init__(args,modules_list,modules_root_dir)
        SalomeAppSLConfig=os.getenv("SalomeAppConfig","")
        os.putenv("SalomeAppSLConfig", SalomeAppSLConfig)
    def getSessionServerExe(self):
        return "SALOME_Session_Server_No_Server"

from .server import process_id
from .addToKillList_impl import addToKillList,killList

def main():
    args, modules_list, modules_root_dir = setenv_impl.get_config()
    mySessionServ = NoSessionServer(args, args.get('modules', []), modules_root_dir)
    runSalomeCommon.setVerbose(mySessionServ.args["verbosity"])
    if mySessionServ.args["killall"]:
        killList()
    logger.debug("Effective args : {}".format(mySessionServ.args))
    mySessionServ.setpath(modules_list, modules_root_dir)
    mySessionServ.run()
    for pid, cmd in list(process_id.items()):
        logger.debug("Killing {} {}".format(pid, cmd))
        addToKillList(pid, cmd)
