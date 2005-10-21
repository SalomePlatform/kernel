#  SALOME SALOME_SWIG : binding of C++ implementation and Python
#
#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#
#
#  File   : salome_test.py
#  Module : SALOME

print "Test the application loading  GEOM, SMESH, VISU, MED, components and doing some"
print "operation within the components."

import salome
from salome import sg
import SALOMEDS
import os

import SALOME_ModuleCatalog

print "======================================================================"
print "           Get Catalog "
print "======================================================================"
obj = salome.naming_service.Resolve('Kernel/ModulCatalog')
catalog = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)

print "======================================================================"
print "           Create Study "
print "======================================================================"

comp = catalog.GetComponent("GEOM")
if comp is None:
	raise RuntimeError,"Component GEOM not found in Module Catalog."

import geompy

print "================================="
print "       create AttributeReal      "
print "================================="
A = geompy.myBuilder.FindOrCreateAttribute(geompy.father, "AttributeReal")
if A == None :
	raise  RuntimeError, "Can't create AttributeReal attribute"
A = A._narrow(SALOMEDS.AttributeReal)
A.SetValue(0.0001)
if A.Value() != 0.0001:
	raise  RuntimeError, "Error : wrong value of  AttributeReal"

print
print " ===========  Test Geometry  =========================="
print

print "==================================="
print "     define a box"
print "==================================="

box = geompy.MakeBox(0., 0., 0., 100., 200., 300.)
idbox = geompy.addToStudy(box,"box")

print
print "=============  Test SMESH  ============================="
print

import StdMeshers

comp = catalog.GetComponent("SMESH")
if comp is None:
	raise RuntimeError,"Component SMESH not found in Module Catalog."

comp = catalog.GetComponent("MED")
if comp is None:
	raise RuntimeError,"Component MED not found in Module Catalog."

import SMESH

geom = salome.lcc.FindOrLoadComponent("FactoryServer", "GEOM")
myBuilder = salome.myStudy.NewBuilder()

smesh = salome.lcc.FindOrLoadComponent("FactoryServer", "SMESH")
smeshgui = salome.ImportComponentGUI("SMESH")
smeshgui.Init(salome.myStudyId);

ShapeTypeCompSolid = 1
ShapeTypeSolid = 2
ShapeTypeShell = 3
ShapeTypeFace = 4
ShapeTypeWire = 5
ShapeTypeEdge = 6
ShapeTypeVertex = 7

# ---- define a box

box = geompy.MakeBox(0., 0., 0., 100., 200., 300.)
idbox = geompy.addToStudy(box,"box")

# ---- add first face of box in study

subShapeList=geompy.SubShapeAll(box,ShapeTypeFace)
face=subShapeList[0]
name = geompy.SubShapeName(face, box)
print name
idface=geompy.addToStudyInFather(box,face,name)

# ---- add shell from box  in study

subShellList=geompy.SubShapeAll(box,ShapeTypeShell)
shell = subShellList[0]
name = geompy.SubShapeName(shell, box)
print name
idshell=geompy.addToStudyInFather(box,shell,name)

# ---- add first edge of face in study

edgeList = geompy.SubShapeAll(face,ShapeTypeEdge)
edge=edgeList[0];
name = geompy.SubShapeName(edge, face)
print name
idedge=geompy.addToStudyInFather(face,edge,name)


# ---- SMESH 

# ---- create Hypothesis

print "-------------------------- create Hypothesis"
print "-------------------------- LocalLength"
hypLen1 = smesh.CreateHypothesis( "LocalLength", "libStdMeshersEngine.so" )
hypLen1.SetLength(100)
print hypLen1.GetName()
print hypLen1.GetId()
print hypLen1.GetLength()

smeshgui.SetName(salome.ObjectToID(hypLen1), "Local_Length_100")

print "-------------------------- NumberOfSegments"
hypNbSeg1= smesh.CreateHypothesis( "NumberOfSegments", "libStdMeshersEngine.so" )
hypNbSeg1.SetNumberOfSegments(7)
print hypNbSeg1.GetName()
print hypNbSeg1.GetId()
print hypNbSeg1.GetNumberOfSegments()

smeshgui.SetName(salome.ObjectToID(hypNbSeg1), "NumberOfSegments_7")

print "-------------------------- MaxElementArea"
hypArea1 = smesh.CreateHypothesis( "MaxElementArea", "libStdMeshersEngine.so" )
hypArea1.SetMaxElementArea(2500)
print hypArea1.GetName()
print hypArea1.GetId()
print hypArea1.GetMaxElementArea()

smeshgui.SetName(salome.ObjectToID(hypArea1), "MaxElementArea_2500")

print "-------------------------- MaxElementArea"
hypArea2 = smesh.CreateHypothesis( "MaxElementArea", "libStdMeshersEngine.so" )
hypArea2.SetMaxElementArea(500)
print hypArea2.GetName()
print hypArea2.GetId()
print hypArea2.GetMaxElementArea()

smeshgui.SetName(salome.ObjectToID(hypArea2), "MaxElementArea_500")

print "-------------------------- Regular_1D"
algoReg = smesh.CreateHypothesis( "Regular_1D", "libStdMeshersEngine.so" )
listHyp=algoReg.GetCompatibleHypothesis()
for hyp in listHyp:
    print hyp
print algoReg.GetName()
print algoReg.GetId()

smeshgui.SetName(salome.ObjectToID(algoReg), "Regular_1D" )

print "-------------------------- MEFISTO_2D"
algoMef = smesh.CreateHypothesis( "MEFISTO_2D", "libStdMeshersEngine.so" )
listHyp=algoMef.GetCompatibleHypothesis()
for hyp in listHyp:
    print hyp
print algoMef.GetName()
print algoMef.GetId()

smeshgui.SetName(salome.ObjectToID(algoMef), "MEFISTO_2D" )

# ---- add hypothesis to box

print "-------------------------- add hypothesis to box"
box=salome.IDToObject(idbox)
mesh = smesh.CreateMesh(box)

smeshgui.SetName( salome.ObjectToID(mesh), "MeshBox" );

ret=mesh.AddHypothesis(box,algoReg)
print ret
ret=mesh.AddHypothesis(box,algoMef)
print ret


ret=mesh.AddHypothesis(box,hypNbSeg1)
print ret
ret=mesh.AddHypothesis(box,hypArea1)
print ret


# ---- add hypothesis to edge

print "-------------------------- add hypothesis to edge"
edge=salome.IDToObject(idedge)
submesh=mesh.GetSubMesh(edge, "SubMeshEdge")

ret=mesh.AddHypothesis(edge,algoReg)
print ret
ret=mesh.AddHypothesis(edge,hypLen1)
print ret

print "-------------------------- add hypothesis to face"
face=salome.IDToObject(idface)
submesh   = mesh.GetSubMesh(face, "SubMeshFace")

ret=mesh.AddHypothesis(face,hypArea2)
print ret

smesh.Compute(mesh, box)
sg.updateObjBrowser(1);

print
print "=============  Test  Supervisor  ============================="
print

comp = catalog.GetComponent("SUPERV")
if comp is None:
	raise RuntimeError,"Component SUPERV not found in Module Catalog."

from SuperV import *
import SALOMEDS
myStudy = salome.myStudy
myBuilder = myStudy.NewBuilder()

SuperVision = lcc.FindOrLoadComponent("SuperVisionContainer","SUPERV")
father = myStudy.FindComponent("SUPERV")
if father is None:
        father = myBuilder.NewComponent("SUPERV")
        A1 = myBuilder.FindOrCreateAttribute(father, "AttributeName");
        FName = A1._narrow(SALOMEDS.AttributeName)
        FName.SetValue( salome.sg.getComponentUserName("SUPERV") )
      	A2 = myBuilder.FindOrCreateAttribute(father, "AttributePixMap");
      	aPixmap = A2._narrow(SALOMEDS.AttributePixMap);
	aPixmap.SetPixMap( "ICON_OBJBROWSER_Supervision" );
	myBuilder.DefineComponentInstance(father,SuperVision)

def addStudy(ior):
    dataflow = SuperVision.getStreamGraph(ior)
    name=dataflow.Name()
    itr = myStudy.NewChildIterator(father)
    while itr.More():
        item=itr.Value()
        res,A=item.FindAttribute("AttributeName")
        if res:
            aName = A._narrow(SALOMEDS.AttributeName)
            if aName.Value() == name :
	        print myBuilder.FindOrCreateAttribute(item, "AttributeIOR")
		A  = myBuilder.FindOrCreateAttribute(item, "AttributeIOR")
		print "A = ", A
		if A is not None :
                   #res,A = myBuilder.FindOrCreateAttribute(item, "AttributeIOR")
                   anIOR  = A._narrow(SALOMEDS.AttributeIOR);
		   print "anIOR.SetValue(dataflow.getIOR())"
		   anIOR.SetValue(dataflow.getIOR()) 
                return
        itr.Next()
    obj = myBuilder.NewObject(father)
    A=myBuilder.FindOrCreateAttribute(obj, "AttributeName")
    aName=A._narrow(SALOMEDS.AttributeName)
    aName.SetValue(name)
    A=myBuilder.FindOrCreateAttribute(obj, "AttributeIOR")
    anIOR  = A._narrow(SALOMEDS.AttributeIOR)
    anIOR.SetValue(dataflow.getIOR())

import os
dir= os.getenv("DATA_DIR")
if dir == None:
	raise RuntimeError, "DATA_DIR is not defined"
xmlfile = dir +"/Superv/Graphs/GraphGeomEssai.xml"
print "Load dataflow from the file : "
print xmlfile
print

myGraph = StreamGraph ( xmlfile )

# This DataFlow is "valid" : no loop, correct links between Nodes etc...
print "myGraph.IsValid() = ", myGraph.IsValid()

# Get Nodes
myGraph.PrintNodes()

# This DataFlow is "executable" : all pending Ports are defined with Datas
print myGraph.IsExecutable()

# Starts only execution of that DataFlow and gets control immediatly
print myGraph.Run()

# That DataFlow is running ==> 0 (false)
print myGraph.IsDone()

# Events of execution :
aStatus,aNode,anEvent,aState = myGraph.Event()
while aStatus :
    print aNode.Thread(),aNode.SubGraph(),aNode.Name(),anEvent,aState
    aStatus,aNode,anEvent,aState = myGraph.Event()
print "myGraph.IsDone() = ",myGraph.IsDone()

# Wait for Completion (but it is already done after event loop ...)
print "Done : ",myGraph.DoneW()

print " "
#print "Type : print myGraph.IsDone()"
#print "       If execution is finished ==> 1 (true)"
res=myGraph.IsDone()
if res != 1:
	raise RuntimeError, "myGraph.Run() is not done"

print " "
print "Type : myGraph.PrintPorts()"
print "       to see input and output values of the graph"
myGraph.PrintPorts()

# Export will create newsupervisionexample.xml and the corresponding .py file
tmpdir=os.getenv("TmpDir")
if tmpdir is None:
	tmpdir="/tmp"
file = tmpdir + "/newsupervisionexample"
print "--------------\n"+file+"\n--------------\n"
myGraph.Export(file)

ior = salome.orb.object_to_string(myGraph.G)
addStudy(ior)

GraphName = myGraph.Name()
print "Befor save ",
#nodes = myGraph.Nodes()
nodes = myGraph.G.Nodes().FNodes
length_bs = len(nodes)
print "ListOfNodes length = ", length_bs
names=[]
for node in nodes:
	names.append(node.Name())
print names

# Graph creation 
GraphInLines = StreamGraph( 'GraphInLines' )
GraphInLines.SetName( 'GraphInLines' )
GraphInLines.SetAuthor( '' )
GraphInLines.SetComment( '' )
GraphInLines.Coords( 0 , 0 )

# Creation of InLine Nodes
PyAdd = []
PyAdd.append( 'def Add(a,b) :  ' )
PyAdd.append( '    return a+b  ' )
PyAdd.append( '' )
Add = GraphInLines.INode( 'Add' , PyAdd )
Add.InPort( 'a' , 'long' )
Add.InPort( 'b' , 'long' )
Add.OutPort( 'f' , 'long' )
Add.SetName( 'Add' )
Add.SetAuthor( '' )
Add.SetComment( 'Python function' )
Add.Coords( 351 , 77 )
PySub = []
PySub.append( 'def Sub(a,b) : ' )
PySub.append( '    return a-b ' )
PySub.append( '' )
Sub = GraphInLines.INode( 'Sub' , PySub )
Sub.InPort( 'a' , 'long' )
Sub.InPort( 'b' , 'long' )
Sub.OutPort( 'f' , 'long' )
Sub.SetName( 'Sub' )
Sub.SetAuthor( '' )
Sub.SetComment( 'Python function' )
Sub.Coords( 86 , 333 )
PyMul = []
PyMul.append( 'def Mul(a,b) : ' )
PyMul.append( '    return a*b ' )
Mul = GraphInLines.INode( 'Mul' , PyMul )
Mul.InPort( 'a' , 'long' )
Mul.InPort( 'b' , 'long' )
Mul.OutPort( 'Result' , 'long' )
Mul.SetName( 'Mul' )
Mul.SetAuthor( '' )
Mul.SetComment( 'Python function' )
Mul.Coords( 616 , 247 )

# Creation of intermediate Output variables and of Control Links
Addf = Add.Port( 'f' )
Mula = GraphInLines.Link( Addf , Mul.Port( 'a' ) )
Mula.AddCoord( 1 , 570 , 356 )
Mula.AddCoord( 2 , 570 , 186 )
Subf = Sub.Port( 'f' )
Mulb = GraphInLines.Link( Subf , Mul.Port( 'b' ) )
Mulb.AddCoord( 1 , 282 , 376 )
Mulb.AddCoord( 2 , 282 , 442 )
Addb = GraphInLines.Link( Subf , Add.Port( 'b' ) )
Addb.AddCoord( 1 , 283 , 209 )
Addb.AddCoord( 2 , 283 , 374 )
Addb.AddCoord( 3 , 283 , 442 )

# Creation of Input datas
Adda = Add.Input( 'a' , 1)
Suba = Sub.Input( 'a' , 3)
Subb = Sub.Input( 'b' , 4)

# Creation of Output variables
MulResult = Mul.Port( 'Result' )

GraphInLines.Run()

GraphInLines.DoneW()

GraphInLines.PrintPorts()

sg.updateObjBrowser(1);

print
print "=============  Test  VISU  and MED ============================="
print

comp = catalog.GetComponent("VISU")
if comp is None:
	raise RuntimeError,"Component VISU not found in Module Catalog."

import sys
import SALOMEDS
import SALOME
import SALOME_MED
import VISU

import visu_gui

medFileName = "pointe.med"
medFile = os.getenv('DATA_DIR') + '/MedFiles/' + medFileName
print "Load ", medFile

studyCurrent = salome.myStudyName

med_comp = salome.lcc.FindOrLoadComponent("FactoryServer", "MED")
myVisu = salome.lcc.FindOrLoadComponent("FactoryServer", "VISU")

try:
    if os.access(medFile, os.R_OK) :
       if not os.access(medFile, os.W_OK) :
	       import random
	       medFileNew = "/tmp/" + str(random.randint(0,1000000)) + "_" + medFileName
	       print " -- Copy " + medFile + " to " + medFileNew
	       os.system("cp "+ medFile + " " + medFileNew)
	       medFile = medFileNew
	       os.system("chmod 755 " + medFile)

       if os.access(medFile, os.W_OK) :
           med_comp.readStructFileWithFieldType(medFile,studyCurrent)
           med_obj = visu_gui.visu.getMedObjectFromStudy()
           print "med_obj - ", med_obj

           myField1 = visu_gui.visu.getFieldObjectFromStudy(2,1)
           aMeshName = "maa1"
           anEntity = VISU.NODE
	   aTimeStampId = -1
           	   
           myResult1 = myVisu.ImportMedField(myField1)
           aMesh1 = myVisu.MeshOnEntity(myResult1, aMeshName, anEntity);
           
	   aScalarMap1= myVisu.ScalarMapOnField(myResult1, aMeshName, anEntity, myField1.getName(), aTimeStampId)
	   
	   myResult2 = myVisu.ImportFile(medFile);
	   aMesh2 = myVisu.MeshOnEntity(myResult2, aMeshName, anEntity);
           
	   aTimeStampId = 3
	   aScalarMap2= myVisu.ScalarMapOnField(myResult2, aMeshName, anEntity, myField1.getName(), aTimeStampId)
    	       	   
	   sg.updateObjBrowser(0)
       else :  print "We have no permission to rewrite medFile, so readStructFileWithFieldType can't open this file";
    else :  print  "We have no permission to read medFile, it will not be opened"; 

except:
    if sys.exc_type == SALOME.SALOME_Exception :
        print "There is no permission to read " + medFile
    else :
        print sys.exc_type 
        print sys.exc_value
        print sys.exc_traceback

sg.updateObjBrowser(1);
