// File:	SALOME_Transformation.hxx
// Created:	Wed Jun  4 09:39:09 2003
// Author:	Alexey PETROV
//		<apo@ivanox.nnov.matra-dtv.fr>


#ifndef SALOME_Transform_HeaderFile
#define SALOME_Transform_HeaderFile

#include <math.h>

#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkDataSetToDataSetFilter.h>
class vtkGeometryFilter;

//=======================================================================

class VTK_EXPORT SALOME_Transform : public vtkTransform{
 public:
  static SALOME_Transform *New();
  vtkTypeMacro(SALOME_Transform,vtkTransform);
  void SetScale(float theScaleX, float theScaleY, float theScaleZ);
  int IsIdentity();
protected:
  SALOME_Transform() {}
  ~SALOME_Transform() {}
  SALOME_Transform(const SALOME_Transform&) {}
  void operator=(const SALOME_Transform&) {}
};

//=======================================================================

class VTK_EXPORT SALOME_TransformFilter : public vtkTransformFilter{
 public:
  static SALOME_TransformFilter *New();
  vtkTypeMacro(SALOME_TransformFilter,vtkTransformFilter);
protected:
  SALOME_TransformFilter() {}
  ~SALOME_TransformFilter() {}
  SALOME_TransformFilter(const SALOME_TransformFilter&) {}
  void operator=(const SALOME_TransformFilter&) {}
  void Execute();
};

//=======================================================================

class SALOME_PassThroughFilter : public vtkDataSetToDataSetFilter{
 public:
  vtkTypeMacro(SALOME_PassThroughFilter,vtkDataSetToDataSetFilter);
  static SALOME_PassThroughFilter *New();
  void SetInput(vtkDataSet *input);
  vtkPolyData *GetPolyDataOutput();
 protected:
  SALOME_PassThroughFilter();
  virtual ~SALOME_PassThroughFilter();
  void Execute();
  vtkGeometryFilter* myGeomFilter;
 private:
  SALOME_PassThroughFilter(const SALOME_PassThroughFilter&);  // Not implemented.
  void operator=(const SALOME_PassThroughFilter&);  // Not implemented.
};

//=======================================================================

#endif
