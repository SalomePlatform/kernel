#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

print
print "Perform quick test of the application by loading of the GEOM, SMESH, VISU, MED"
print "components and doing some operation within the components."
print

import salome
import SALOME
import SALOMEDS
import os
import sys
import SALOME_ModuleCatalog

step = 1

print "======================================================================"
print "           %d. Initialize study " % step; step+=1
print "======================================================================"

# initialize study
salome.salome_init()
# get study builder
builder = salome.myStudy.NewBuilder()
print "OK"

print 

print "======================================================================"
print "           %d. Retrieve module catalog " % step; step+=1
print "======================================================================"

obj = salome.naming_service.Resolve('Kernel/ModulCatalog')
catalog = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)
if not catalog:
    raise RuntimeError, "Can't accesss module catalog"
print "OK"

print

print "======================================================================"
print "           %d. Check modules availability in the module catalog " % step; step+=1
print "======================================================================"

print
print "--- Check GEOM ..."
comp = catalog.GetComponent("GEOM")
if not comp:
    raise RuntimeError, "Component GEOM is not found in Module Catalog."
print "OK"

print
print "--- Check SMESH ..."
comp = catalog.GetComponent("SMESH")
if not comp:
    raise RuntimeError, "Component SMESH is not found in Module Catalog."
print "OK"

print
print "--- Check MED ..."
comp = catalog.GetComponent("MED")
if not comp:
    raise RuntimeError, "Component MED is not found in Module Catalog."
print "OK"

print
print "--- Check VISU ..."
comp = catalog.GetComponent("VISU")
if not comp:
    raise RuntimeError, "Component VISU is not found in Module Catalog."
print "OK"

print

print "======================================================================"
print "           %d. Test Data Server " % step; step+=1
print "======================================================================"

print
print "--- Create new component ..."
comp = builder.NewComponent("TEST")
if not comp:
    raise RuntimeError, "Can't create new component"
print "OK"

print
print "--- Create AttributeName ..."
A = builder.FindOrCreateAttribute(comp, "AttributeName")
if not A:
    raise RuntimeError, "Can't create AttributeName attribute"
A.SetValue("TEST")
if A.Value() != "TEST":
    raise RuntimeError, "Error : wrong value of  AttributeName"
print "OK"

print
print "--- Create AttributeReal ..."
A = builder.FindOrCreateAttribute(comp, "AttributeReal")
if not A:
    raise RuntimeError, "Can't create AttributeReal attribute"
A.SetValue(0.0001)
if A.Value() != 0.0001:
    raise RuntimeError, "Error : wrong value of  AttributeReal"
print "OK"

print

print "======================================================================"
print "           %d. Test Geometry " % step; step+=1
print "======================================================================"

import geompy

ShapeTypeCompSolid = 1
ShapeTypeSolid = 2
ShapeTypeShell = 3
ShapeTypeFace = 4
ShapeTypeWire = 5
ShapeTypeEdge = 6
ShapeTypeVertex = 7

print
print "--- Create a box ..."
box = geompy.MakeBox(0., 0., 0., 100., 200., 300.)
idbox = geompy.addToStudy(box, "box")
box_obj = salome.myStudy.FindObjectByPath("/Geometry/box")
if not box_obj:
    raise RuntimeError, "Error : wrong value of  AttributeReal"
print "OK"

# ---- add shell from box in study
print 
print "--- Extract shell ..."
subShellList = geompy.SubShapeAll(box, ShapeTypeShell)
shell = subShellList[0]
name = geompy.SubShapeName(shell, box)
idshell = geompy.addToStudyInFather(box, shell, name)
print name
print "OK"

# ---- add first face of box in study
print 
print "--- Extract face ..."
subShapeList = geompy.SubShapeAll(box, ShapeTypeFace)
face = subShapeList[0]
name = geompy.SubShapeName(face, box)
idface = geompy.addToStudyInFather(box, face, name)
print name
print "OK"

# ---- add first edge of face in study
print 
print "--- Extract edge ..."
edgeList = geompy.SubShapeAll(face, ShapeTypeEdge)
edge = edgeList[0];
name = geompy.SubShapeName(edge, face)
idedge = geompy.addToStudyInFather(face, edge, name)
print name
print "OK"

# ---- update object browser
if salome.hasDesktop():
    salome.sg.updateObjBrowser(1);

print

print "======================================================================"
print "           %d. Test Mesh " % step; step+=1
print "======================================================================"

import StdMeshers
import SMESH

smesh = salome.lcc.FindOrLoadComponent("FactoryServer", "SMESH")
if salome.hasDesktop():
    smeshgui = salome.ImportComponentGUI("SMESH")
    smeshgui.Init(salome.myStudyId);
else:
    smesh.SetCurrentStudy(salome.myStudy)

# ---- create hypotheses 

if sys.platform == "win32":
  stdMeshersEngine = "StdMeshersEngine"
else:
  stdMeshersEngine = "libStdMeshersEngine.so"

print
print "--- Create hypotheses ..."

print
print "------ LocalLength ..."
hypLen1 = smesh.CreateHypothesis( "LocalLength", stdMeshersEngine )
hypLen1.SetLength(100)
print hypLen1.GetName()
print hypLen1.GetId()
print hypLen1.GetLength()
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(hypLen1), "Local_Length_100")
print "OK"

print
print "------ NumberOfSegments ..."
hypNbSeg1= smesh.CreateHypothesis( "NumberOfSegments", stdMeshersEngine )
hypNbSeg1.SetNumberOfSegments(7)
print hypNbSeg1.GetName()
print hypNbSeg1.GetId()
print hypNbSeg1.GetNumberOfSegments()
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(hypNbSeg1), "NumberOfSegments_7")
print "OK"

print
print "------ MaxElementArea [1] ..."
hypArea1 = smesh.CreateHypothesis( "MaxElementArea", stdMeshersEngine )
hypArea1.SetMaxElementArea(2500)
print hypArea1.GetName()
print hypArea1.GetId()
print hypArea1.GetMaxElementArea()
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(hypArea1), "MaxElementArea_2500")
print "OK"

print
print "------ MaxElementArea [2] ..."
hypArea2 = smesh.CreateHypothesis( "MaxElementArea", stdMeshersEngine )
hypArea2.SetMaxElementArea(500)
print hypArea2.GetName()
print hypArea2.GetId()
print hypArea2.GetMaxElementArea()
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(hypArea2), "MaxElementArea_500")
print "OK"

# ---- create algorithms

print
print "--- Create algorithms ..."

print
print "------ Regular_1D ..."
algoReg = smesh.CreateHypothesis( "Regular_1D", stdMeshersEngine )
listHyp = algoReg.GetCompatibleHypothesis()
for hyp in listHyp:
    print hyp
print algoReg.GetName()
print algoReg.GetId()
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(algoReg), "Regular_1D" )
print "OK"

print
print "------ MEFISTO_2D ..."
algoMef = smesh.CreateHypothesis( "MEFISTO_2D", stdMeshersEngine )
listHyp=algoMef.GetCompatibleHypothesis()
for hyp in listHyp:
    print hyp
print algoMef.GetName()
print algoMef.GetId()
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(algoMef), "MEFISTO_2D" )
print "OK"

# ---- create mesh on the box, apply hypotheses / algorithms

print
print "--- Create mesh on the box ..."
mesh = smesh.CreateMesh(box)
if salome.hasDesktop():
    smeshgui.SetName( salome.ObjectToID(mesh), "MeshBox" );
ret = mesh.AddHypothesis(box, algoReg)
ret = mesh.AddHypothesis(box, algoMef)
ret = mesh.AddHypothesis(box, hypNbSeg1)
ret = mesh.AddHypothesis(box, hypArea1)
print "OK"

# ---- create submesh on the edge, add hypothesis

print
print "--- Add 1D sub-mesh on the edge ..."
submesh = mesh.GetSubMesh(edge, "SubMeshEdge")
ret = mesh.AddHypothesis(edge, algoReg)
ret = mesh.AddHypothesis(edge, hypLen1)
print "OK"

# ---- create submesh on the edge, add hypothesis

print
print "--- Add 2D sub-mesh on the face ..."
submesh = mesh.GetSubMesh(face, "SubMeshFace")
ret = mesh.AddHypothesis(face, hypArea2)
print "OK"

# ---- compute mesh
print
print "--- Compute mesh ..."
smesh.Compute(mesh, box)
print "OK"

# ---- update object browser
if salome.hasDesktop():
    salome.sg.updateObjBrowser(1);

print

print "======================================================================"
print "           %d. Test Med " % step; step+=1
print "======================================================================"

import SALOME_MED

medFileName = "pointe.med"
medFile = os.path.join(os.getenv('DATA_DIR'), 'MedFiles', medFileName)

med_comp = salome.myStudy.FindComponent("MED")

if not med_comp:
    med  = salome.lcc.FindOrLoadComponent("FactoryServer", "MED")

    print
    print "--- Read med file structure from %s ..." % medFile
    med.readStructFileWithFieldType(medFile, salome.myStudyName)
    print "OK"
    pass
else:
    print
    print "This script cannot work properly, because there is"
    print "some MED component data already existing in the study."
    print "Execution aborted."
    print
    print "Skipping test for MED..."
    pass

print

print "======================================================================"
print "           %d. Test Post-Pro " % step; step+=1
print "======================================================================"

import VISU

aMeshName = "maa1"
anEntity = VISU.NODE
field_name = "fieldnodedouble"

if salome.hasDesktop(): # in gui mode
    
    import visu_gui
    visu = salome.lcc.FindOrLoadComponent("FactoryServer", "VISU")

    if not med_comp:
        
        print
        print "--- Get med object from study ..."
        med_obj = visu_gui.visu.getMedObjectFromStudy()
        if not med_obj:
            raise RuntimeError, "Med object is not found in the study"
        print "OK"

        print
        print "--- Get field from study ..."
        field = visu_gui.visu.getFieldObjectFromStudy(3,1)
        if not field:
            raise RuntimeError, "Field object is not found in the study"
        print "OK"

        print
        print "--- Import field to the VISU ..."
        aTimeStampId = -1
        result1 = visu.ImportMedField(field)
        if not result1:
            raise RuntimeError, "Can't import field"
        print "OK"

        print
        print "--- Create mesh presentation ..."
        mesh1 = visu.MeshOnEntity(result1, aMeshName, anEntity);
        if not mesh1:
            raise RuntimeError, "Can't create mesh presentation"
        print "OK"

        print
        print "--- Create scalar map ..."
        scalarMap1 = visu.ScalarMapOnField(result1, aMeshName, anEntity, field_name, aTimeStampId)
        if not scalarMap1:
            raise RuntimeError, "Can't create scalar map"
        print "OK"

        pass # if not med_comp

    print
    print "--- Import med file %s to the VISU ..." % medFile
    result2 = visu.ImportFile(medFile);
    if not result2:
        raise RuntimeError, "Can't import file"
    print "OK"

    print
    print "--- Create mesh presentation ..."
    mesh2 = visu.MeshOnEntity(result2, aMeshName, anEntity);
    if not mesh2:
        raise RuntimeError, "Can't create mesh presentation"
    print "OK"

    print
    print "--- Create scalar map ..."
    scalarMap2 = visu.ScalarMapOnField(result2, aMeshName, anEntity, field_name, 3)
    if not scalarMap2:
        raise RuntimeError, "Can't create scalar map"
    print "OK"
    pass

else: # not in gui mode, visu can not be tested
    
    print
    print "VISU module requires SALOME to be running in GUI mode."
    print
    print "Skipping test for VISU..."
    pass

# ---- update object browser
if salome.hasDesktop():
    salome.sg.updateObjBrowser(1);
