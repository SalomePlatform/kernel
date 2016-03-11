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
Typical use is:
  python update_catalogs.py

You need to have a well installed SALOME application with a CatalogResources.base.xml file.
This file is used (parsed) to collect all module catalogs from distant resources and
put them in the directory "remote_catalogs" with sub-directories with same name as the distant resource.
Distant resources are all the resources except the main SALOME application.
Module catalogs from distant resources are copied by the remote protocol declared in the catalog (rcp or rsh)
except for the user resources on the local machine (local copy: cp).

In a second time, this procedure generates a ready to use CatalogResources.xml with all available components
for each resource.
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

catalog_file_base=os.path.join(appli_local,"CatalogResources.base.xml")
catalog_file=os.path.join(appli_local,"CatalogResources.xml")

cata_dir=os.path.join(appli_local,"remote_catalogs")
cata_dir_bak=os.path.join(appli_local,"remote_catalogs.bak")

SEP=":"
if sys.platform == "win32":SEP=";"

def get_hostname():
  return socket.gethostname().split('.')[0]

class Component:
  """Define a SALOME component
      - name : component name
      - moduleName : module name
  """
  def __init__(self,name,moduleName):
    self.name=name
    self.moduleName=moduleName

class Resource:
  """Define a SALOME resource
     - components : the list of available components of the resource
  """
  def __init__(self,node):
    self.node=node
    self.components=[]
    self.resource_dir=None
    self.build()

  def build(self):
    self.attrs=self.node.attrib
    #remove all children (components and modules)
    for child in list(self.node):
      self.node.remove(child)

  def update(self):
    for compo in self.components:
      child=etree_.Element("component",name=compo.name,moduleName=compo.moduleName)
      child.tail="\n"
      self.node.append(child)

  def get_rcp(self):
    protocol= self.node.get("protocol")
    if protocol and protocol[0]=='s':return "scp"
    else:return "rcp"

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

  def get_catalogs(self):
    """Get module catalogs file from the resource and copy them locally in remote_catalogs/<resource name>"""
    hostname=self.get_host()
    appliPath= self.get_appliPath()
    userName = self.get_user()
    rcopy=self.get_rcp()

    resource_dir=os.path.join(cata_dir,self.get_name())

    if hostname == "localhost" or hostname == get_hostname() and userName == getUserName():
      #local machine, use cp
      if appliPath[0]!='/':
        #relative path
        appliPath=os.path.join(os.getenv("HOME"),appliPath)

      if appliPath == appli_local:
        return
      os.mkdir(resource_dir)
      cata_path=os.path.join(appliPath,"share","salome","resources","*Catalog.xml")
      cmd="cp %s %s" % (cata_path,resource_dir)
      print cmd
      os.system(cmd)
      cata_path=os.path.join(appliPath,"share","salome","resources","*","*Catalog.xml")
      cmd="cp %s %s" % (cata_path,resource_dir)
      print cmd
      os.system(cmd)
    else:
      #remote machine, use rcopy
      os.mkdir(resource_dir)
      cata_path=os.path.join(appliPath,"share","salome","resources","*Catalog.xml")
      cmd="%s %s@%s:%s %s"
      cmd= cmd%(rcopy,userName,hostname,cata_path,resource_dir)
      print cmd
      os.system(cmd)
      cata_path=os.path.join(appliPath,"share","salome","resources","*","*Catalog.xml")
      cmd="%s %s@%s:%s %s"
      cmd= cmd%(rcopy,userName,hostname,cata_path,resource_dir)
      print cmd
      os.system(cmd)

    schema_cata=os.path.join(resource_dir,"*SchemaCatalog.xml")
    os.system("rm %s"% schema_cata)

    self.resource_dir=os.path.abspath(resource_dir)

  def get_components(self):
    """Retrieve all components declared in module catalogs of the resource"""
    appliPath= self.get_appliPath()
    userName = self.get_user()
    hostname=self.get_host()
    resource_dir=os.path.join(cata_dir,self.get_name())
    catalogs_list=glob.glob(os.path.join(resource_dir,"*Catalog.xml"))

    if hostname == "localhost" or hostname == get_hostname() and userName == getUserName():
      #user local resource
      if appliPath[0]!='/':
        appliPath=os.path.join(os.getenv("HOME"),appliPath)
      if appliPath == appli_local:
        #main local resource: get catalogs in share/salome/resources
        catalogs_list=glob.glob(os.path.join(appliPath,"share","salome","resources","*","*Catalog.xml"))
        catalogs_list=catalogs_list + glob.glob(os.path.join(appliPath,"share","salome","resources","*Catalog.xml"))

    for cata in catalogs_list:
      moduleName= os.path.basename(cata)[:-11]
      #Parse module catalog
      doc = etree_.parse(cata)
      rootNode = doc.getroot()
      for componentName in rootNode.findall("component-list/component/component-name"):
        self.components.append(Component(componentName.text,moduleName))


def main():
  parser = optparse.OptionParser(usage=usage)

  options, args = parser.parse_args()

  if not os.path.exists(catalog_file_base):
    print "ERROR: the base catalog file %s is mandatory" % catalog_file_base
    sys.exit(1)

  #Parse CatalogResource.xml
  doc = etree_.parse(catalog_file_base)

  rootNode = doc.getroot()
  if rootNode.tag != "resources":
    raise  ParseError("First level tag must be resources not %s" % rootNode.tag)

  resources=[]

  #Extract resources
  for child in rootNode:
    if child.tag != "machine":
      raise  ParseError("Second level tag must be machine not %s" % child.tag)
    resources.append(Resource(child))

  # Remove remote_catalogs directory and create a new empty one
  if os.path.exists(cata_dir):
    if os.path.exists(cata_dir_bak):
      shutil.rmtree(cata_dir_bak)
    os.rename(cata_dir,cata_dir_bak)

  os.mkdir(cata_dir)

  #Get catalogs from remote resources and copy them in remote_catalogs
  for mach in resources:
    mach.get_catalogs()

  #Get the list of SALOME components that are defined in catalogs
  for mach in resources:
    mach.get_components()

  #Update the resource catalog dom object for further dump
  for mach in resources:
    mach.update()

  #dump new CatalogResources.xml
  f=open(catalog_file,'w')
  f.write('<?xml version="1.0" ?>\n')
  doc.write(f)
  f.write('\n')
  f.close()
  print "%s updated" % catalog_file

  #update configRemote.sh in env.d directory (environment variable SALOME_CATALOGS_PATH)
  path=[]
  for mach in resources:
    if mach.resource_dir:
      path.append(mach.resource_dir)

  f=open(os.path.join(appli_local,"env.d","configRemote.sh"),'w')
  f.write("export SALOME_CATALOGS_PATH=%s\n" % SEP.join(path))
  f.close()


if __name__ == '__main__':
  main()

