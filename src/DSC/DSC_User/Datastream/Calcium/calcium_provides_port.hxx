// Eric Fayolle - EDF R&D
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (Wed, 28 Feb 2007) $
// Id          : $Id$

#ifndef _CALCIUM_PROVIDES_PORT_HXX_
#define _CALCIUM_PROVIDES_PORT_HXX_

#include "provides_port.hxx"
#include "CalciumTypes.hxx"

class calcium_provides_port : public provides_port
{
public :

  typedef CalciumTypes::DependencyType       DependencyType;
  typedef CalciumTypes::DateCalSchem         DateCalSchem;
  typedef CalciumTypes::InterpolationSchem   InterpolationSchem;
  typedef CalciumTypes::ExtrapolationSchem   ExtrapolationSchem;
  typedef CalciumTypes::DisconnectDirective  DisconnectDirective;  

  calcium_provides_port();
  virtual ~calcium_provides_port();

  virtual void           setDependencyType (DependencyType dependencyType) =0;
  virtual DependencyType getDependencyType () const =0;
 
  virtual  void   setStorageLevel   (size_t storageLevel) =0;
  virtual  size_t getStorageLevel   () const =0;

  virtual  void         setDateCalSchem   (DateCalSchem   dateCalSchem) =0;
  virtual  DateCalSchem getDateCalSchem () const =0;
  
  virtual  void   setAlpha(double alpha) =0;
  virtual  double getAlpha() const  =0;

  virtual  void   setDeltaT(double deltaT ) =0;
  virtual  double getDeltaT() const  =0;

  virtual  void setInterpolationSchem (InterpolationSchem interpolationSchem) =0;
  virtual  void setExtrapolationSchem (ExtrapolationSchem extrapolationSchem) =0;
  virtual  InterpolationSchem getInterpolationSchem () const  =0;
  virtual  ExtrapolationSchem getExtrapolationSchem () const  =0;

};

#endif

