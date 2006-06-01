#  Copyright (C) 2003  CEA/DEN, EDF R&D
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#
#  File   : batchmode_salome.py
#  Module : SALOME

import salome_shared_modules

from omniORB import CORBA
from LifeCycleCORBA import *
import SALOME
import SALOMEDS
from SALOME_NamingServicePy import *

#--------------------------------------------------------------------------

def ImportComponentGUI(ComponentName):
    libName = "lib" + ComponentName + "_Swig"
    command = "from " + libName + " import *"
    exec ( command )
    constructor = ComponentName + "GUI_Swig()"
    command = "gui = " + constructor
    exec ( command )
    return gui

    #--------------------------------------------------------------------------

def SalomeGUIgetAllSelected(self):
    selNumber = self.SelectedCount()
    listSelected = []
    for i in range(selNumber):
        listSelected.append(self.getSelected(i))
    return listSelected
    #--------------------------------------------------------------------------

def generateName(prefix = None):
    import whrandom;
    int = whrandom.randint(1,1000);
    if prefix is None:
        return "Study" + str(int)
    else :
        return prefix + str(int)

#WITHOUTIHMfrom libSALOME_Swig import *
###from SalomePyQt import *
#WITHOUTIHMclass SalomeGUI(SALOMEGUI_Swig):
    #WITHOUTIHMgetAllSelected = SalomeGUIgetAllSelected
   
    #WITHOUTIHMdef getDesktop(self) :
#	return SalomePyQt.getDesktop()
	#WITHOUTIHMreturn None

    #WITHOUTIHMdef getSelection(self) :
#	return SalomePyQt.getSelection()
	#WITHOUTIHMreturn None

    #--------------------------------------------------------------------------

def IDToObject(id):
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
    mySO = myStudy.FindObjectID(id);
    return mySO

    #--------------------------------------------------------------------------

def PersistentPresentation(theStudy, theSO, theWithID):
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
    anIter = theStudy.NewChildIterator(theSO)
    while anIter.More():
        aResult += PersistentPresentation(theStudy, anIter.Value(), theWithID)
        anIter.Next()
    return aResult

    #--------------------------------------------------------------------------

def GetTree(theSO):
    # returns the document list tree (as list)
    aResult = [theSO.GetID()]
    anIter = myStudy.NewChildIterator(theSO)
    while anIter.More():
        aResult += GetTree(anIter.Value())
        anIter.Next()
    return aResult

    #--------------------------------------------------------------------------

def CheckCopyPaste(theSO, theInfo ,theComponentPaste):
    aRoot = theSO
    while aRoot.GetID() != "0:":
        aRoot = aRoot.GetFather()
    aTree = GetTree(aRoot)
    aStudyPersist = PersistentPresentation(myStudy, aRoot, 1)

    if not myStudyManager.CanCopy(theSO):
        raise RuntimeError, "<CanCopy> for "+theInfo+" returns false"
    
    if not myStudyManager.Copy(theSO):
        raise RuntimeError, "<Copy> for "+theInfo+" returns false"

    
    if not myStudyManager.CanPaste(theSO):
        raise RuntimeError, "<CanPaste> for "+theInfo+" returns false"

    # check: before paste study is not changed check
    if aStudyPersist != PersistentPresentation(myStudy, aRoot, 1):
        raise RuntimeError, "Study is changed before Paste calling for "+theInfo
    
    aSObj = theSO
    if theComponentPaste:
        aSObj = theSO.GetFatherComponent()
        theInfo = theInfo + "(paste for component)"
    if myStudyManager.Paste(aSObj) == None:
        raise RuntimeError, "<Paste> for "+theInfo+" returns None object"
    aNewTree = GetTree(aRoot)
    aLen = len(aTree)
    for a in range(0,aLen):
        if aTree[a] != aNewTree[a]:
            return myStudy.FindObjectID(aNewTree[a])
        
    if aLen < len(aNewTree):
        return myStudy.FindObjectID(aNewTree[aLen])
    
    raise RuntimeError, "After Copy calling the tree is not changed"
    
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
# initialise the ORB
orb = None

step = 0
while step < 100 and orb is None:
    orb = CORBA.ORB_init([''], CORBA.ORB_ID)
    step = step + 1
    time.sleep(4)

if orb is None:
    print "Warning: ORB has not been initialized !!!"

# create an LifeCycleCORBA instance
lcc = LifeCycleCORBA(orb)

step = 0
while step < 100 and lcc is None:
    lcc = LifeCycleCORBA(orb)
    step = step + 1
    time.sleep(4)
    
if lcc is None:
    print "Warning: LifeCycleCORBA object has not been initialized !!!"
    
#create a naming service instance
naming_service = SALOME_NamingServicePy_i(orb)

# get Study Manager reference
obj = None

step = 0
while step < 100 and obj == None:
    obj = naming_service.Resolve('myStudyManager')
    step = step + 1
    time.sleep(4)
     
myStudyManager = obj._narrow(SALOMEDS.StudyManager)

if myStudyManager is None:
    print "Warning: SALOMEDS.StudyManager has not been created !!!"

# create new study
aListOfOpenStudies = myStudyManager.GetOpenStudies();
myStudy = None;
if len(aListOfOpenStudies) == 0 :
    myStudy = myStudyManager.NewStudy("Study1")
else:
    myStudyName = aListOfOpenStudies[0]
    myStudy = myStudyManager.GetStudyByName(myStudyName)
    
myStudyName = myStudy._get_Name()

myStudyId = myStudy._get_StudyId()
print myStudyId

