#!/usr/bin/env python
#  -*- coding: utf-8 -*-
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

"""
"""
import sys,os,shutil,glob,socket
import optparse
from salome_utils import getUserName

import getAppliPath
appli_local=os.path.realpath(os.path.dirname(__file__))
APPLI=getAppliPath.relpath(appli_local,os.path.realpath(os.getenv('HOME')))

usage="""usage: %prog [options]

This procedure kill all containers that have been launched in a SALOME session on remote machines.
A SALOME session is identified by a machine name and a port number.

You need to have a well installed SALOME application with a CatalogResources.xml file.
This file is used (parsed) to collect all remote resources.
Remote resources are all the resources on remote machines not including the main local SALOME application.
"""

try:
  # cElementTree from Python 2.5+
  import xml.etree.cElementTree as etree_
except ImportError:
  try:
    import xml.etree.ElementTree as etree_
  except ImportError:
    try:
      import cElementTree as etree_
    except ImportError:
      try:
        # normal ElementTree install
        import elementtree.ElementTree as etree_
      except ImportError:
        raise

class ParseError(Exception):
  pass

catalog_file=os.path.join(appli_local,"CatalogResources.xml")

SEP=":"
if sys.platform == "win32":SEP=";"

def get_hostname():
  return socket.gethostname().split('.')[0]

class Resource:
  """Define a SALOME resource
     - components : the list of available components of the resource
  """
  def __init__(self,node):
    self.node=node
    self.components=[]
    self.resource_dir=None

  def get_rcp(self):
    protocol= self.node.get("protocol")
    if protocol and protocol[0]=='s':return "scp"
    else:return "rcp"

  def get_rsh(self):
    protocol= self.node.get("protocol")
    if protocol and protocol[0]=='s':return "ssh"
    else:return "rsh"

  def get_user(self):
    userName= self.node.get("userName")
    if not userName:
      userName=getUserName()
    return userName

  def get_host(self):
    hostname= self.node.get("hostname")
    return hostname

  def get_name(self):
    name= self.node.get("name")
    if name:return name
    return self.get_host()

  def get_appliPath(self):
    appliPath= self.node.get("appliPath")
    if appliPath is None:
      appliPath=APPLI
    return appliPath

def main():
  parser = optparse.OptionParser(usage=usage)
  parser.add_option('-p','--port', dest="port",
                            help="The SALOME session port (default NSPORT or 2810)")


  options, args = parser.parse_args()

  if not os.path.exists(catalog_file):
    print "ERROR: the catalog file %s is mandatory" % catalog_file_base
    sys.exit(1)

  #Parse CatalogResource.xml
  doc = etree_.parse(catalog_file)

  rootNode = doc.getroot()
  if rootNode.tag != "resources":
    raise  ParseError("First level tag must be resources not %s" % rootNode.tag)

  resources=[]

  #Extract resources
  for child in rootNode:
    if child.tag != "machine":
      raise  ParseError("Second level tag must be machine not %s" % child.tag)
    resources.append(Resource(child))

  local_names=("localhost",get_hostname())

  for resource in resources:
    if resource.get_host() in local_names:continue
    command=resource.get_rsh() +" -l "+resource.get_user()+" "+resource.get_host()
    command=command+ " " + os.path.join(resource.get_appliPath(),"runRemote.sh")
    if options.port:
      port=options.port
    else:
      port=os.getenv("NSPORT") or "2810"
    command=command+ " " + get_hostname() + " " + port +" killSalomeWithPort.py " + port
    print command
    os.system(command)

if __name__ == '__main__':
  main()

