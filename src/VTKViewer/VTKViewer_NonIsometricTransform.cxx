using namespace std;
// File      : VTKViewer_NonIsometricTransform.cxx
// Created   : Thu Jan 30 15:21:07 2003
// Project   : SALOME PRO
// Author    : Vasily RUSYAEV (vrv)
// Copyright : Open CASCADE 2003
//  $Header:

#include "VTKViewer_NonIsometricTransform.h"

double ident(double in, double coeff) { return in; }
double dident(double in, double coeff) { return 1.0; }

double Exponent(double in, double coeff){ return exp(in)-coeff;}
double dExponent(double in, double coeff){ return exp(in);}
double Log(double in, double coeff){ return log(in+coeff);}
double dLog(double in, double coeff){ return 1.0/(in+coeff);}

double Scl(double in, double coeff){ return coeff*in;}
double dScl(double in, double coeff){ return coeff;}
double IScl(double in, double coeff){ return in/coeff;}
double dIScl(double in, double coeff){ return 1.0/coeff;}


AxisFunction::AxisFunction(Axis A)
     : m_axis(A), fFun(&ident), dfFun(&dident), iFun(&ident), diFun(&dident)
{
}
AxisFunction::AxisFunction()
     : m_axis(XAxis), fFun(&ident), dfFun(&dident), iFun(&ident), diFun(&dident)
{
}
     
inline void AxisFunction::forward(const double in, const double coeff, double& out, double der[3]) const
{
  init_der(der);
  der[m_axis] = (*dfFun)( in, coeff);
  forward(in, coeff, out);
}
  
inline void AxisFunction::inverse(const double in, const double coeff, double& out, double der[3]) const
{
  init_der(der);
  der[m_axis] = (*diFun)( in, coeff );
  inverse(in, coeff , out);
}

inline MFun AxisFunction::getfFun()
{
  return fFun;
}  

VTKViewer_NonIsometricTransform::VTKViewer_NonIsometricTransform ()
{
  m_coeff[0] = m_coeff[1] = m_coeff[2] = 1.0;
  m_axisFunctions[0] = AxisFunction ( AxisFunction::XAxis );
  m_axisFunctions[1] = AxisFunction ( AxisFunction::YAxis );
  m_axisFunctions[2] = AxisFunction ( AxisFunction::ZAxis );
}

void VTKViewer_NonIsometricTransform::ForwardTransformPoint(const float in[3], float out[3])
{
  //TODO
  //ForwardTransformPoint( in, out );
}
inline void VTKViewer_NonIsometricTransform::ForwardTransformPoint(const double in[3], double out[3])
{
  for(int i = 0; i < 3; i++) 
    m_axisFunctions[i].forward(in[i], m_coeff[i], out[i]);
}
inline void VTKViewer_NonIsometricTransform::ForwardTransformDerivative(const double in[3],
									double out[3], double der[3][3])
{
  for(int i = 0; i < 3; i++)
    m_axisFunctions[i].forward(in[i], m_coeff[i], out[i], der[i]);
}
void VTKViewer_NonIsometricTransform::ForwardTransformDerivative(const float in[3], float out[3], float der[3][3])
{
  //TODO
  //ForwardTransformDerivative(in,out,der);
}
inline void VTKViewer_NonIsometricTransform::InverseTransformDerivative(const double in[3],
									double out[3], double der[3][3])
{
  for(int i = 0; i < 3; i++)
    m_axisFunctions[i].inverse(in[i], m_coeff[i], out[i], der[i]);
}
inline void VTKViewer_NonIsometricTransform::InverseTransformPoint(const double in[3], double out[3])
{
  for(int i = 0; i < 3; i++)
    m_axisFunctions[i].inverse(in[i], m_coeff[i], out[i]);
}
void VTKViewer_NonIsometricTransform::InverseTransformPoint(const float in[3], float out[3])
{
  //TODO
  //InverseTransformPoint(in,out);
}


void VTKViewer_NonIsometricTransform::SetFunction(const AxisFunction::Axis axis,
						  const Function aFunction, const double coeff)
{
  m_coeff[axis] = coeff;
  switch (aFunction)
    {
    case Identical:
      m_axisFunctions[axis] = AxisFunction( axis );
      break;
    case Linear:
      m_axisFunctions[axis] = AxisFunction( axis, &Scl,&dScl,&IScl,&dIScl);
      break;
    case Logarithmic:
      m_axisFunctions[axis] = AxisFunction( axis, &Log, &dLog, &Exponent, &dExponent );
      break;
    }	  
}

inline void VTKViewer_NonIsometricTransform::SetFunction(const AxisFunction AF)
{
  m_axisFunctions[AF.getAxis()] = AF;
}

VTKViewer_NonIsometricTransform* VTKViewer_NonIsometricTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("VTKViewer_NonIsometricTransform");
  if(ret)
    return (VTKViewer_NonIsometricTransform*)ret;
  // If the factory was unable to create the object, then create it here.
  return new VTKViewer_NonIsometricTransform;
}

VTKViewer_NonIsometricTransform::Function VTKViewer_NonIsometricTransform::GetFunctionType(const AxisFunction::Axis axis)
{
  if ( m_axisFunctions[axis].getfFun() == &ident) {
    return Identical;
  }
  if ( m_axisFunctions[axis].getfFun() == &Scl ) {
    return Linear;
  }
  if ( m_axisFunctions[axis].getfFun() == &Log ) {
    return Logarithmic;
  }
}

double VTKViewer_NonIsometricTransform::GetCoeff  (const AxisFunction::Axis axis) 
{
  return m_coeff[axis];
}
