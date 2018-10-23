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

print()
print("Perform quick test of the application by loading of the GEOM, SMESH, FIELDS, PARAVIS")
print("components and doing some operation within the components.")
print()

import salome
import SALOME
import SALOMEDS
import os
import sys
import time
import SALOME_ModuleCatalog

step = 1

print("======================================================================")
print("           %d. Initialize study " % step); step+=1
print("======================================================================")

# initialize study
salome.salome_init()
# get study builder
builder = salome.myStudy.NewBuilder()
print("OK")

print() 

print("======================================================================")
print("           %d. Retrieve module catalog " % step); step+=1
print("======================================================================")

obj = salome.naming_service.Resolve('Kernel/ModulCatalog')
catalog = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)
if not catalog:

    raise RuntimeError("Can't access module catalog")
print("OK")

print()

print("======================================================================")
print("           %d. Check modules availability in the module catalog " % step); step+=1
print("======================================================================")

for module in [ "GEOM", "SMESH", "MEDFactory", "PVSERVER"]:
    print()
    print("--- Check %s ..." % module)
    comp = catalog.GetComponent(module)
    if not comp:
        raise RuntimeError("Component %s is not found in Module Catalog." % module)
    print("OK")
    pass

print()

print("======================================================================")
print("           %d. Test Data Server " % step); step+=1
print("======================================================================")

print()
print("--- Create new component ...")
comp = builder.NewComponent("TEST")
if not comp:
    raise RuntimeError("Can't create new component")
print("OK")

print()
print("--- Create AttributeName ...")
A = builder.FindOrCreateAttribute(comp, "AttributeName")
if not A:
    raise RuntimeError("Can't create AttributeName attribute")
A.SetValue("TEST")
if A.Value() != "TEST":
    raise RuntimeError("Error : wrong value of  AttributeName")
print("OK")

print()
print("--- Create AttributeReal ...")
A = builder.FindOrCreateAttribute(comp, "AttributeReal")
if not A:
    raise RuntimeError("Can't create AttributeReal attribute")
A.SetValue(0.0001)
if A.Value() != 0.0001:
    raise RuntimeError("Error : wrong value of  AttributeReal")
print("OK")

print()

print("======================================================================")
print("           %d. Test Geometry " % step); step+=1
print("======================================================================")

from salome.geom import geomBuilder
geompy = geomBuilder.New()

ShapeTypeCompSolid = 1
ShapeTypeSolid = 2
ShapeTypeShell = 3
ShapeTypeFace = 4
ShapeTypeWire = 5
ShapeTypeEdge = 6
ShapeTypeVertex = 7

print()
print("--- Create a box ...")
box = geompy.MakeBox(0., 0., 0., 100., 200., 300.)
idbox = geompy.addToStudy(box, "box")
box_obj = salome.myStudy.FindObjectByPath("/Geometry/box")
if not box_obj:
    raise RuntimeError("Error : wrong value of  AttributeReal")
print("OK")

# ---- add shell from box in study
print() 
print("--- Extract shell ...")
subShellList = geompy.SubShapeAll(box, ShapeTypeShell)
shell = subShellList[0]
name = geompy.SubShapeName(shell, box)
idshell = geompy.addToStudyInFather(box, shell, name)
print(name)
print("OK")

# ---- add first face of box in study
print() 
print("--- Extract face ...")
subShapeList = geompy.SubShapeAll(box, ShapeTypeFace)
face = subShapeList[0]
name = geompy.SubShapeName(face, box)
idface = geompy.addToStudyInFather(box, face, name)
print(name)
print("OK")

# ---- add first edge of face in study
print() 
print("--- Extract edge ...")
edgeList = geompy.SubShapeAll(face, ShapeTypeEdge)
edge = edgeList[0];
name = geompy.SubShapeName(edge, face)
idedge = geompy.addToStudyInFather(face, edge, name)
print(name)
print("OK")

# ---- update object browser
if salome.hasDesktop():
    salome.sg.updateObjBrowser();

print()

print("======================================================================")
print("           %d. Test Mesh " % step); step+=1
print("======================================================================")

from salome.StdMeshers import StdMeshersBuilder
import SMESH
from salome.smesh import smeshBuilder

smesh = salome.lcc.FindOrLoadComponent("FactoryServer", "SMESH")
if salome.hasDesktop():
    smeshgui = salome.ImportComponentGUI("SMESH")
    smeshgui.Init();
else:
    smesh = smeshBuilder.New()

# ---- create hypotheses 

stdMeshersEngine = "StdMeshersEngine"

print()
print("--- Create hypotheses ...")

print()
print("------ LocalLength ...")
hypLen1 = smesh.CreateHypothesis( "LocalLength", stdMeshersEngine )
hypLen1.SetLength(100)
print(hypLen1.GetName())
print(hypLen1.GetId())
print(hypLen1.GetLength())
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(hypLen1), "Local_Length_100")
print("OK")

print()
print("------ NumberOfSegments ...")
hypNbSeg1= smesh.CreateHypothesis( "NumberOfSegments", stdMeshersEngine )
hypNbSeg1.SetNumberOfSegments(7)
print(hypNbSeg1.GetName())
print(hypNbSeg1.GetId())
print(hypNbSeg1.GetNumberOfSegments())
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(hypNbSeg1), "NumberOfSegments_7")
print("OK")

print()
print("------ MaxElementArea [1] ...")
hypArea1 = smesh.CreateHypothesis( "MaxElementArea", stdMeshersEngine )
hypArea1.SetMaxElementArea(2500)
print(hypArea1.GetName())
print(hypArea1.GetId())
print(hypArea1.GetMaxElementArea())
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(hypArea1), "MaxElementArea_2500")
print("OK")

print()
print("------ MaxElementArea [2] ...")
hypArea2 = smesh.CreateHypothesis( "MaxElementArea", stdMeshersEngine )
hypArea2.SetMaxElementArea(500)
print(hypArea2.GetName())
print(hypArea2.GetId())
print(hypArea2.GetMaxElementArea())
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(hypArea2), "MaxElementArea_500")
print("OK")

# ---- create algorithms

print()
print("--- Create algorithms ...")

print()
print("------ Regular_1D ...")
algoReg = smesh.CreateHypothesis( "Regular_1D", stdMeshersEngine )
listHyp = algoReg.GetCompatibleHypothesis()
for hyp in listHyp:
    print(hyp)
print(algoReg.GetName())
print(algoReg.GetId())
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(algoReg), "Regular_1D" )
print("OK")

print()
print("------ MEFISTO_2D ...")
algoMef = smesh.CreateHypothesis( "MEFISTO_2D", stdMeshersEngine )
listHyp=algoMef.GetCompatibleHypothesis()
for hyp in listHyp:
    print(hyp)
print(algoMef.GetName())
print(algoMef.GetId())
if salome.hasDesktop():
    smeshgui.SetName(salome.ObjectToID(algoMef), "MEFISTO_2D" )
print("OK")

# ---- create mesh on the box, apply hypotheses / algorithms

print()
print("--- Create mesh on the box ...")
mesh = smesh.CreateMesh(box)
if salome.hasDesktop():
    smeshgui.SetName( salome.ObjectToID(mesh), "MeshBox" );
ret = mesh.AddHypothesis(box, algoReg)
ret = mesh.AddHypothesis(box, algoMef)
ret = mesh.AddHypothesis(box, hypNbSeg1)
ret = mesh.AddHypothesis(box, hypArea1)
print("OK")

# ---- create submesh on the edge, add hypothesis

print()
print("--- Add 1D sub-mesh on the edge ...")
submesh = mesh.GetSubMesh(edge, "SubMeshEdge")
ret = mesh.AddHypothesis(edge, algoReg)
ret = mesh.AddHypothesis(edge, hypLen1)
print("OK")

# ---- create submesh on the edge, add hypothesis

print()
print("--- Add 2D sub-mesh on the face ...")
submesh = mesh.GetSubMesh(face, "SubMeshFace")
ret = mesh.AddHypothesis(face, hypArea2)
print("OK")

# ---- compute mesh
print()
print("--- Compute mesh ...")
smesh.Compute(mesh, box)
print("OK")

# ---- update object browser
if salome.hasDesktop():
    salome.sg.updateObjBrowser();

print()

print("======================================================================")
print("           %d. Test Fields " % step); step+=1
print("======================================================================")

import medcalc
#from medcalc import properties
from medcalc.fieldproxy import FieldProxy

medcalc.medconsole.setConsoleGlobals(globals())
try:
    med_root=os.environ["FIELDS_ROOT_DIR"]
except KeyError as e:
    raise RuntimeError("FIELDS_ROOT_DIR should be defined to load the test data")

filepath = os.path.join(med_root,"share","salome","resources","fields","medcalc_testfiles","smallmesh_varfield.med")
medcalc.medio.LoadDataSource(filepath)
fieldHandlerList = medcalc.medevents.dataManager.getFieldHandlerList()

fieldHandler0 = fieldHandlerList[0]
print("---Field Handler 0:\n%s" % fieldHandler0)
fieldHandler1 = fieldHandlerList[1]
print("---Field Handler 1:\n%s" % fieldHandler1)

print("--- The addition of two fields can be done using field handler directly.")
addFieldHandler = medcalc.fieldproxy.calculator.add(fieldHandler0,fieldHandler1)
print("--- Result handler:\n%s" % addFieldHandler)

print("--- Or with a field proxy that easy the writing of operations.")
fieldProxy0 = FieldProxy(fieldHandler0)
fieldProxy1 = FieldProxy(fieldHandler1)

resHandler = fieldProxy0 + fieldProxy1
if resHandler is None:
    print("Error: result handler is None!")
else:
    print("--- Result handler:\n%s" % resHandler)
    print("OK")

print()


print("======================================================================")
print("           %d. Test Paravis " % step); step+=1
print("======================================================================")

if salome.hasDesktop(): # in gui mode

    print("**** Importing pvserver... It can take some time.")
    import pvsimple as pvs

    #====================Stage1: Importing MED file====================

    print("**** Stage1: Importing MED file")
    
    med_file = "ResOK_0000.med"
    print("Import '{}'...............".format(med_file), end=' ')
    path = os.path.join(os.getenv('DATA_DIR'), 'MedFiles', med_file)
    med_reader = pvs.MEDReader(FileName=path)
    if med_reader is None:
        print("FAILED")
    else:
        print("OK")
    times = med_reader.TimestepValues.GetData()

    #====================Stage2: Displaying presentation===============
    
    print("**** Stage2: Displaying presentation")

    # -----------------------------------------------------------------
    print('Get view...................', end=' ')
    view = pvs.GetRenderView()
    if view is None:
        print("FAILED")
    else:
        print("OK")
    view.ResetCamera()

    # -----------------------------------------------------------------
    print("Show mesh..................", end=' ')
    mesh = pvs.Show(med_reader, view)
    view.ResetCamera()
    if mesh is None:
        print("FAILED")
    else:
        print("OK")
    mesh.Representation = 'Surface With Edges'
    pvs.Render(view)
    time.sleep(1)

    # -----------------------------------------------------------------
    print("Show scalar map............", end=' ')
    scalar_map = mesh
    if scalar_map is None:
        print("FAILED")
    else:
        print("OK")
    scalar_map.Representation = 'Surface'
    pvs.ColorBy(scalar_map, ('POINTS', 'vitesse', 'Magnitude'))
    #scalar_map.SetScalarBarVisibility(view, True)
    view.ViewTime = times[-1]
    pvs.Render(view)
    time.sleep(1)
    scalar_map.Visibility = 0
    pvs.Render(view)

    # -----------------------------------------------------------------
    print("Show vectors...............", end=' ')
    calc = pvs.Calculator(Input=med_reader)
    calc.ResultArrayName = 'vitesse_3c'
    calc.Function = 'iHat * vitesse_X + jHat * vitesse_Y + kHat * 0'
    glyph = pvs.Glyph(Input=calc, GlyphType='Arrow')
    glyph.OrientationArray = ['POINTS', 'vitesse_3c']
    glyph.ScaleArray = ['POINTS', 'No scale array']
    glyph.MaximumGlyphSize = 0.01
    vectors = pvs.Show(glyph, view)
    if vectors is None:
        print("FAILED")
    else:
        print("OK")
    vectors.Representation = 'Surface'
    pvs.Render(view)
    time.sleep(1)
    vectors.Visibility = 0
    pvs.Render(view)

    # -----------------------------------------------------------------
    print("Show iso surfaces..........", end=' ')
    merge_blocks = pvs.MergeBlocks(Input=med_reader)
    calc = pvs.Calculator(Input=merge_blocks)
    calc.ResultArrayName = 'vitesse_magnitude'
    calc.Function = 'sqrt(vitesse_X^2+vitesse_Y^2)'
    data_range = med_reader.GetPointDataInformation()['vitesse'].GetComponentRange(-1)
    nb_surfaces = 10
    surfaces = [data_range[0] + i*(data_range[1]-data_range[0])/(nb_surfaces-1) for i in range(nb_surfaces)]
    contour = pvs.Contour(Input=calc)
    contour.ComputeScalars = 1
    contour.ContourBy = ['POINTS', 'vitesse_magnitude']
    contour.Isosurfaces = surfaces
    iso_surfaces = pvs.Show(contour, view)
    if iso_surfaces is None:
        print("FAILED")
    else:
        print("OK")
    iso_surfaces.Representation = 'Surface'
    pvs.ColorBy(iso_surfaces, ('POINTS', 'vitesse', 'Magnitude'))
    pvs.Render(view)
    time.sleep(1)
    iso_surfaces.Visibility = 0
    pvs.Render(view)

    # -----------------------------------------------------------------
    print("Show cut planes............", end=' ')
    slice = pvs.Slice(Input=med_reader)
    slice.SliceType = "Plane"
    slice.SliceType.Normal = [1.0, 0.0, 0.0]
    bounds = med_reader.GetDataInformation().GetBounds()
    nb_planes = 30
    displacement = 0.5
    b_left = bounds[0] + (bounds[1]-bounds[0])*displacement/100
    b_right = bounds[1] - (bounds[1]-bounds[0])*displacement/100
    b_range = b_right - b_left
    positions = [b_left + i*b_range/(nb_planes-1) for i in range(nb_planes)]
    slice.SliceOffsetValues = positions
    pvs.Hide3DWidgets(proxy=slice.SliceType)
    cut_planes = pvs.Show(slice, view)
    if cut_planes is None:
        print("FAILED")
    else:
        print("OK")
    cut_planes.Representation = 'Surface'
    pvs.ColorBy(cut_planes, ('POINTS', 'vitesse', 'Magnitude'))
    pvs.Render(view)
    time.sleep(1)
    cut_planes.Visibility = 0
    pvs.Render(view)

    # -----------------------------------------------------------------
    print("Show deformed shape........", end=' ')
    merge_blocks = pvs.MergeBlocks(Input=med_reader)
    calc = pvs.Calculator(Input=merge_blocks)
    calc.ResultArrayName = 'vitesse_3c'
    calc.Function = 'iHat * vitesse_X + jHat * vitesse_Y + kHat * 0'
    warp = pvs.WarpByVector(Input=calc)
    warp.Vectors = ['POINTS', 'vitesse_3c']
    warp.ScaleFactor = 0.5
    deformed_shape = pvs.Show(warp, view)
    if deformed_shape is None:
        print("FAILED")
    else:
        print("OK")
    deformed_shape.Representation = 'Surface'
    pvs.ColorBy(deformed_shape, ('CELLS', 'pression'))
    pvs.Render(view)
    time.sleep(1)
    deformed_shape.Visibility = 0
    pvs.Render(view)

else: # not in gui mode, Paravis can not be tested
    
    print()
    print("PARAVIS module requires SALOME to be running in GUI mode.")
    print()
    print("Skipping test for PARAVIS...")
    pass

# ---- update object browser
if salome.hasDesktop():
    salome.sg.updateObjBrowser();
