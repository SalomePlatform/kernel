import libSalomePy
ren=libSalomePy.renderer
#iren=libSalomePy.interactor

from libVTKCommonPython import *
from libVTKGraphicsPython import *
cone = vtkConeSource()
cone.SetResolution(8)
coneMapper = vtkPolyDataMapper()
coneMapper.SetInput(cone.GetOutput())
coneActor = vtkActor()
coneActor.SetMapper(coneMapper)

ren.AddActor(coneActor)
iren = vtkRenderWindowInteractor()

