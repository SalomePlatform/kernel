#ifndef __VTKViewer_NonIsometricTransform_h
#define __VTKViewer_NonIsometricTransform_h


#include <vtkWarpTransform.h>
//#include <vtkImageData.h>
//#include <vtkTransformPolyDataFilter.h>

#include <vtkObjectFactory.h>
#include <math.h>

//second parameter is coefficient ( e.g y = ky, second parameter is k )
typedef double (*MFun)(double,double);

class AxisFunction
{
 public:
  enum Axis {XAxis=0, YAxis, ZAxis};
 private:
  
  void init_der(double der[3]) const
    {
      der[XAxis] = der[YAxis] = der[ZAxis] = 0.0;
    }

 public:
  AxisFunction(Axis A, MFun fF, MFun dfF, MFun iF, MFun diF)
    : m_axis(A), fFun(fF), dfFun(dfF), iFun(iF), diFun(diF) {};
  AxisFunction(Axis A);
  AxisFunction();

  const Axis getAxis() const{ return m_axis; }
  void setAxis(const Axis axis) { m_axis = axis; }

  void forward(const double in, const double coeff, double& out) const
    {
      out = (*fFun)( in, coeff );
    }
  
  void forward(const double in, const double coeff, double& out, double der[3]) const;
  
  void inverse(const double in, const double coeff, double& out) const
    {
      out = (*iFun)( in, coeff );
    }
  void inverse(const double in, const double coeff, double& out, double der[3]) const;
  
  MFun getfFun() ;

 private:
  Axis m_axis;
  MFun fFun, dfFun, iFun, diFun;
};


class VTK_EXPORT VTKViewer_NonIsometricTransform : public vtkWarpTransform
{
 public:  
  enum Function { Identical = 0, Linear, Logarithmic };
 protected:
  
  void ForwardTransformPoint(const float in[3], float out[3]);
  void ForwardTransformPoint(const double in[3], double out[3]);
  void ForwardTransformDerivative(const double in[3], double out[3], double der[3][3]);
  void ForwardTransformDerivative(const float in[3], float out[3], float der[3][3]);
  void InverseTransformDerivative(const double in[3], double out[3], double der[3][3]);
  void InverseTransformPoint(const double in[3], double out[3]);
  void InverseTransformPoint(const float in[3], float out[3]);

  VTKViewer_NonIsometricTransform();

  void SetFunction(const AxisFunction AF);

 public:
  
  vtkTypeMacro(VTKViewer_NonIsometricTransform,vtkWarpTransform);
  static VTKViewer_NonIsometricTransform *New();

  vtkAbstractTransform* MakeTransform()
    {
      return VTKViewer_NonIsometricTransform::New();
    }
  virtual void PrintSelf(ostream& os, vtkIndent indent)
    {
      vtkWarpTransform::PrintSelf(os,indent);
    }
  void SetFunction(const AxisFunction::Axis axis, const Function aFunction, const double coeff);

  Function GetFunctionType(const AxisFunction::Axis axis);
  double GetCoeff (const AxisFunction::Axis axis);

 private:
  AxisFunction m_axisFunctions[3];
  double m_coeff[3];
};

#endif
