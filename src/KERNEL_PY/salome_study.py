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

#  File   : salome_study.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
import salome_kernel
import SALOMEDS
import salome_iapp
from launchConfigureParser import verbose

myStudy = None
myStudyName = None

#--------------------------------------------------------------------------

def DumpComponent(SO, Builder,offset):
  global myStudy
  it = myStudy.NewChildIterator(SO)
  while it.More():
    CSO = it.Value()
    a=offset*"--" + ">" + CSO.GetID()
    find,AtName = Builder.FindAttribute(CSO, "AttributeName")
    if find:
      a=a+":"+AtName.Value()
    find,AtIOR = Builder.FindAttribute(CSO, "AttributeIOR")
    if find:
      a=a+":"+AtIOR.Value()
    find,RefSO = CSO.ReferencedObject()
    if find:
      a=a+":"+RefSO.GetID()
    print(a)
    DumpComponent(CSO, Builder,offset+2)
    it.Next()

#--------------------------------------------------------------------------

def DumpStudy():
    """
    Dump a study, given the ior
    """
    global myStudy
    itcomp = myStudy.NewComponentIterator()
    Builder = myStudy.NewBuilder()
    while itcomp.More():
      SC = itcomp.Value()
      name = SC.ComponentDataType()
      print("-> ComponentDataType is " + name)
      DumpComponent(SC,Builder, 1)
      itcomp.Next()

#--------------------------------------------------------------------------

def IDToObject(id):
    global myStudy
    myObj = None
    mySO = myStudy.FindObjectID(id);
    if mySO is not None:
        ok, anAttr = mySO.FindAttribute("AttributeIOR")
        if ok:
            AtIOR = anAttr._narrow(SALOMEDS.AttributeIOR)
            if AtIOR.Value() != "":
                myObj = orb.string_to_object(AtIOR.Value())
    return myObj

def ObjectToSObject(obj):
    global myStudy
    mySO = None
    if obj is not None:
        ior =  orb.object_to_string(obj)
        if ior != "":
            mySO = myStudy.FindObjectIOR(ior)
    return mySO

def ObjectToID(obj):
    mySO = ObjectToSObject(obj)
    if mySO:
        return mySO.GetID()
    return ""

def IDToSObject(id):
    global myStudy
    mySO = myStudy.FindObjectID(id);
    return mySO

    #--------------------------------------------------------------------------

def generateName(prefix = None):
    import random;
    int = random.randint(1,1000);
    if prefix is None:
        return "Study" + str(int)
    else :
        return prefix + str(int)

    #--------------------------------------------------------------------------

def PersistentPresentation(theSO, theWithID):
    global myStudy
    # put the sobject's content (with subchildren) to the string
    aResult = ""
    attrs = theSO.GetAllAttributes()
    aLen = len(attrs)
    anUncopied = 0
    for a in range(0,aLen):
        attr = attrs[a]
        if isinstance(attr,SALOMEDS._objref_AttributeTreeNode):
            anUncopied += 1
        elif isinstance(attr,SALOMEDS._objref_AttributeTarget):
            anUncopied += 1
        elif isinstance(attr,SALOMEDS._objref_AttributeReal) or \
             isinstance(attr,SALOMEDS._objref_AttributeInteger) or \
             isinstance(attr,SALOMEDS._objref_AttributeName) or \
             isinstance(attr,SALOMEDS._objref_AttributeComment) or \
             isinstance(attr,SALOMEDS._objref_AttributePersistentRef) or \
             isinstance(attr,SALOMEDS._objref_AttributeLocalID) or \
             isinstance(attr,SALOMEDS._objref_AttributeUserID):
            aResult += " attribute value: " + str(attr.Value())
        elif isinstance(attr,SALOMEDS._objref_AttributeIOR):
            aResult += " attribute: IOR"
        elif isinstance(attr,SALOMEDS._objref_AttributeSequenceOfReal) or \
             isinstance(attr,SALOMEDS._objref_AttributeSequenceOfInteger):
            aResult += " Sequence: " + str(attr.CorbaSequence())
        elif isinstance(attr,SALOMEDS._objref_AttributeDrawable):
            aResult += " Drawable: " + str(attr.IsDrawable())
        elif isinstance(attr,SALOMEDS._objref_AttributeSelectable):
            aResult += " Selectable: " + str(attr.IsSelectable())
        elif isinstance(attr,SALOMEDS._objref_AttributeExpandable):
            aResult += " Expandable: " + str(attr.IsExpandable())
        elif isinstance(attr,SALOMEDS._objref_AttributeOpened):
            aResult += " Opened: " + str(attr.IsOpened())
        elif isinstance(attr,SALOMEDS._objref_AttributeTextColor):
            aResult += " TextColor: " + str(attr.TextColor())
        elif isinstance(attr,SALOMEDS._objref_AttributeTextHighlightColor):
            aResult += " TextHighlightColor: " + str(attr.TextHighlightColor())
        elif isinstance(attr,SALOMEDS._objref_AttributePixMap):
            aResult += " PixMap: " + str(attr.GetPixMap())
        elif isinstance(attr,SALOMEDS._objref_AttributeTableOfInteger) or \
             isinstance(attr,SALOMEDS._objref_AttributeTableOfReal):
            aResult += " Table with title: " + attr.GetTitle()
        elif isinstance(attr,SALOMEDS._objref_AttributePythonObject):
            aResult += " PythonObject: " + attr.GetObject()

    if theWithID:
        aResult = "sobject: " + theSO.GetID() + " nbattrs: " + str(aLen - anUncopied) + aResult + '\n'
    else:
        aResult = " nbattrs: " + str(aLen - anUncopied) + aResult + '\n'
    anIter = myStudy.NewChildIterator(theSO)
    while anIter.More():
        aResult += PersistentPresentation(anIter.Value(), theWithID)
        anIter.Next()
    return aResult

    #--------------------------------------------------------------------------

def GetTree(theSO):
    # returns the document list tree (as list)
    global myStudy
    aResult = [theSO.GetID()]
    anIter = myStudy.NewChildIterator(theSO)
    while anIter.More():
        aResult += GetTree(anIter.Value())
        anIter.Next()
    return aResult

    #--------------------------------------------------------------------------

def CheckCopyPaste(theSO, theInfo ,theComponentPaste):
    global myStudy

    aRoot = theSO
    while aRoot.GetID() != "0:":
        aRoot = aRoot.GetFather()
    aTree = GetTree(aRoot)
    aStudyPersist = PersistentPresentation(aRoot, 1)

    if not myStudy.CanCopy(theSO):
        raise RuntimeError("<CanCopy> for "+theInfo+" returns false")
    
    if not myStudy.Copy(theSO):
        raise RuntimeError("<Copy> for "+theInfo+" returns false")

    
    if not myStudy.CanPaste(theSO):
        raise RuntimeError("<CanPaste> for "+theInfo+" returns false")

    # check: before paste study is not changed check
    if aStudyPersist != PersistentPresentation(aRoot, 1):
        raise RuntimeError("Study is changed before Paste calling for "+theInfo)
    
    aSObj = theSO
    if theComponentPaste:
        aSObj = theSO.GetFatherComponent()
        theInfo = theInfo + "(paste for component)"
    if myStudy.Paste(aSObj) == None:
        raise RuntimeError("<Paste> for "+theInfo+" returns None object")
    aNewTree = GetTree(aRoot)
    aLen = len(aTree)
    for a in range(0,aLen):
        if aTree[a] != aNewTree[a]:
            return myStudy.FindObjectID(aNewTree[a])
        
    if aLen < len(aNewTree):
        return myStudy.FindObjectID(aNewTree[aLen])
    
    raise RuntimeError("After Copy calling the tree is not changed")
    
    #--------------------------------------------------------------------------

def GetComponentVersion(theComponent, all_versions = False):
    # returns the document list tree (as list)
    global myStudy
    props = myStudy.GetProperties()
    stored_components = props.GetStoredComponents()
    version = "no component data" # vsr: better raise an exception in this case?
    if theComponent in stored_components:
      if all_versions:
        version = props.GetComponentVersions(theComponent)
        for i in range(len(version)):
          if not version[i]: version[i] = "unknown"
          pass
        pass
      else:
        version = props.GetComponentVersion(theComponent)
        if not version: version = "unknown"
        pass
      pass
    return version
    
    #--------------------------------------------------------------------------

def FindFileInDataDir(filename):
    import os
    datadir = os.getenv("DATA_DIR")
    if datadir is not None:
        import string
        dirs = string.split(datadir, ":")
        for dir in dirs:
            file = dir + "/" + filename
            if os.path.exists(file):
                return file;
    datadir = os.getenv("KERNEL_ROOT_DIR") + "/examples/"
    file = datadir + filename
    if os.path.exists(file):
        return file;

    return None

    #--------------------------------------------------------------------------

def openStudy(theStudyPath):
    if verbose(): print("openStudy (begin)")
    global myStudy, myStudyName
    myStudy.Open(theStudyPath)
    myStudyName = myStudy._get_Name()
    if verbose(): print("openStudy (end):", theStudyPath, myStudy._get_Name())

    #--------------------------------------------------------------------------

def salome_study_init(theStudyPath=None):
    """
    Performs only once study creation or connection.
    optional argument : theStudyPath
        None        : attach to the currently active single study;
                      create new empty study if there is active study
        <URL> (str) : open study with the given file name
    """
    global myStudy, myStudyName
    global orb, lcc, naming_service, cm

    if verbose(): print("theStudyPath:", theStudyPath)
    if not myStudy:
        orb, lcc, naming_service, cm = salome_kernel.salome_kernel_init()

        # get Study reference
        if verbose(): print("looking for study...")
        obj = naming_service.Resolve('/Study')
        myStudy = obj._narrow(SALOMEDS.Study)
        if verbose(): print("Study found")
        pass

    import types
    if theStudyPath and isinstance(theStudyPath, (str, bytes)):
        if isinstance(theStudyPath, bytes):
            theStudyPath = str(theStudyPath, 'UTF8')
        openStudy(theStudyPath)
    else:
        myStudy.Init()

    myStudyName = myStudy._get_Name()

    return myStudy, myStudyName

def salome_study_close():
  global myStudy, myStudyName
  myStudy, myStudyName = None, None
