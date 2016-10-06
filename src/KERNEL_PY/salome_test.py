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

print
print "Perform quick test of the application by loading of the GEOM, SMESH, MED, PARAVIS"
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

for module in [ "GEOM", "SMESH", "MEDFactory", "PVSERVER"]:
    print
    print "--- Check %s ..." % module
    comp = catalog.GetComponent(module)
    if not comp:
        raise RuntimeError, "Component %s is not found in Module Catalog." % module
    print "OK"
    pass

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

from salome.geom import geomBuilder
geompy = geomBuilder.New(salome.myStudy)

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
    salome.sg.updateObjBrowser(True);

print

print "======================================================================"
print "           %d. Test Mesh " % step; step+=1
print "======================================================================"

from salome.StdMeshers import StdMeshersBuilder
import SMESH
from salome.smesh import smeshBuilder

smesh = salome.lcc.FindOrLoadComponent("FactoryServer", "SMESH")
if salome.hasDesktop():
    smeshgui = salome.ImportComponentGUI("SMESH")
    smeshgui.Init(salome.myStudyId);
else:
    smesh = smeshBuilder.New(salome.myStudy)

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
    salome.sg.updateObjBrowser(True);

print

print "======================================================================"
print "           %d. Test Med " % step; step+=1
print "======================================================================"

import medcalc
#from medcalc import properties
from medcalc.fieldproxy import FieldProxy

medcalc.medconsole.setConsoleGlobals(globals())
try:
    med_root=os.environ["MED_ROOT_DIR"]
except KeyError, e:
    raise RuntimeError("MED_ROOT_DIR should be defined to load the test data")

filepath = os.path.join(med_root,"share","salome","resources","med","medcalc_testfiles","smallmesh_varfield.med")
medcalc.medio.LoadDataSource(filepath)
fieldHandlerList = medcalc.medevents.dataManager.getFieldHandlerList()

fieldHandler0 = fieldHandlerList[0]
print "---Field Handler 0:\n%s" % fieldHandler0
fieldHandler1 = fieldHandlerList[1]
print "---Field Handler 1:\n%s" % fieldHandler1

print "--- The addition of two fields can be done using field handler directly."
addFieldHandler = medcalc.fieldproxy.calculator.add(fieldHandler0,fieldHandler1)
print "--- Result handler:\n%s" % addFieldHandler

print "--- Or with a field proxy that easy the writing of operations."
fieldProxy0 = FieldProxy(fieldHandler0)
fieldProxy1 = FieldProxy(fieldHandler1)

resHandler = fieldProxy0 + fieldProxy1
if resHandler is None:
    print "Error: result handler is None!"
else:
    print "--- Result handler:\n%s" % resHandler
    print "OK"

print


print "======================================================================"
print "           %d. Test Paravis " % step; step+=1
print "======================================================================"

if salome.hasDesktop(): # in gui mode

    print "**** Importing pvserver... It can take some time."
    from presentations import *
    import pvserver
    import pvsimple
    
    my_paravis = pvserver.myPVServerService
    
    #====================Stage1: Importing MED file====================
    
    print "**** Stage1: Importing MED file"
    
    print 'Import "ResOK_0000.med"...............',
    medFileName = "ResOK_0000.med"
    medFile = os.path.join(os.getenv('DATA_DIR'), 'MedFiles', medFileName)
    pvsimple.MEDReader( FileName=medFile )
    med_reader = pvsimple.GetActiveSource()
    
    if med_reader is None:
        print "FAILED"
    else:
        print "OK"
    
    cell_entity = EntityType.CELL
    node_entity = EntityType.NODE
    
    #====================Stage2: Displaying vector field===============
    
    print "**** Stage3: Displaying vector field"
    
    print 'Get view...................',
    view = pvsimple.GetRenderView()
    if view is None:
        print "FAILED"
    else:
        reset_view(view)
        print "OK"
    
    print "Creating Scalar Map.......",
    scalarmap = ScalarMapOnField(med_reader, node_entity, 'vitesse', 2)
    if scalarmap is None:
        print "FAILED"
    else:
        bar = get_bar()
        bar.Orientation = 'Horizontal'
        bar.Position = [0.1, 0.1]
        bar.Position2 = [0.1, 0.25]
        bar.AspectRatio = 3
        
        display_only(scalarmap, view)
        print "OK"
    
    view.ResetCamera()
    
    print "Creating Vectors..........",
    vectors = VectorsOnField(med_reader, node_entity, 'vitesse', 2)
    if vectors is None:
        print "FAILED"
    else:
        display_only(vectors, view)
        print "OK"
    
    print "Creating Iso Surfaces.....",
    isosurfaces = IsoSurfacesOnField(med_reader, node_entity, 'vitesse', 2)
    if isosurfaces is None:
        print "FAILED"
    else:
        display_only(isosurfaces, view)
        print "OK"
    
    print "Creating Cut Planes.......",
    cutplanes = CutPlanesOnField(med_reader, node_entity, 'vitesse', 2,
                                 nb_planes=30, orientation=Orientation.YZ)
    if cutplanes is None:
        print "FAILED"
    else:
        display_only(cutplanes, view)
        print "OK"
    
    print "Creating Scalar Map On Deformed Shape.......",
    scalarmapondefshape = DeformedShapeAndScalarMapOnField(med_reader,
                                                           node_entity,
                                                           'vitesse', 2,
                                                           None,
                                                           cell_entity,
                                                           'pression')
    if scalarmapondefshape is None:
        print "FAILED"
    else:
        display_only(scalarmapondefshape, view)
        print "OK"

else: # not in gui mode, Paravis can not be tested
    
    print
    print "PARAVIS module requires SALOME to be running in GUI mode."
    print
    print "Skipping test for PARAVIS..."
    pass

# ---- update object browser
if salome.hasDesktop():
    salome.sg.updateObjBrowser(True);
