//  SALOME OCCViewer : build OCC Viewer into Salome desktop
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : OCCViewer_VService.h
//  Module : SALOME

#ifndef _OCCViewer_VService_HeaderFile
#define _OCCViewer_VService_HeaderFile

#ifndef _Handle_V3d_View_HeaderFile
#include <Handle_V3d_View.hxx>
#endif
#ifndef _Handle_V2d_View_HeaderFile
#include <Handle_V2d_View.hxx>
#endif
#ifndef _Handle_V2d_Viewer_HeaderFile
#include <Handle_V2d_Viewer.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Xw_WindowQuality_HeaderFile
#include <Xw_WindowQuality.hxx>
#endif
#ifndef _Handle_V3d_Viewer_HeaderFile
#include <Handle_V3d_Viewer.hxx>
#endif
#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
#ifndef _Standard_ExtString_HeaderFile
#include <Standard_ExtString.hxx>
#endif
#ifndef _Quantity_Length_HeaderFile
#include <Quantity_Length.hxx>
#endif
#ifndef _Quantity_NameOfColor_HeaderFile
#include <Quantity_NameOfColor.hxx>
#endif
#ifndef _V3d_TypeOfOrientation_HeaderFile
#include <V3d_TypeOfOrientation.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_Graphic2d_View_HeaderFile
#include <Handle_Graphic2d_View.hxx>
#endif
#ifndef _Handle_Aspect_WindowDriver_HeaderFile
#include <Handle_Aspect_WindowDriver.hxx>
#endif

class V3d_View;
class V3d_Viewer;

class V2d_View;
class V2d_Viewer;
class Graphic2d_View;
class Aspect_WindowDriver;

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class OCCViewer_VService  {

public:

    inline void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    inline void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    inline void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }

// Methods PUBLIC
// 
Standard_EXPORT static  void SetWindow(const Handle(V3d_View)& aView,const Standard_Integer hiwin,const Standard_Integer lowin,const Xw_WindowQuality aQuality = Xw_WQ_3DQUALITY) ;
Standard_EXPORT static  void SetMagnify(const Handle(V3d_View)& aView,const Standard_Integer hiwin,const Standard_Integer lowin,const Handle(V3d_View)& aPreviousView,const Standard_Integer x1,const Standard_Integer y1,const Standard_Integer x2,const Standard_Integer y2,const Xw_WindowQuality aQuality = Xw_WQ_3DQUALITY) ;
Standard_EXPORT static  Handle_V3d_Viewer Viewer(const Standard_CString aDisplay,const Standard_ExtString aName,const Standard_CString aDomain = "",const Quantity_Length ViewSize = 1000.0,const V3d_TypeOfOrientation ViewProj = V3d_XposYnegZpos,const Standard_Boolean ComputedMode = Standard_True,const Standard_Boolean DefaultComputedMode = Standard_True) ;

Standard_EXPORT static  Handle_V2d_View XView(const Handle(V2d_Viewer)& aViewer,const Standard_Integer hiwin,const Standard_Integer lowin,const Xw_WindowQuality aQuality = Xw_WQ_DRAWINGQUALITY,const Standard_Boolean Update = Standard_True,const Quantity_NameOfColor BackColor = Quantity_NOC_MATRAGRAY) ;
Standard_EXPORT static  Handle_V2d_View XdpsView(const Handle(V2d_Viewer)& aViewer,const Standard_Integer hiwin,const Standard_Integer lowin,const Xw_WindowQuality aQuality = Xw_WQ_DRAWINGQUALITY,const Standard_Boolean Update = Standard_True,const Quantity_NameOfColor BackColor = Quantity_NOC_MATRAGRAY) ;
Standard_EXPORT static  Handle_V2d_Viewer Viewer(const Standard_CString aDisplay,const Standard_ExtString aName,const Standard_CString aDomain = "") ;
Standard_EXPORT static  Handle_V2d_Viewer Viewer(const Standard_CString aDisplay,const Handle(Graphic2d_View)& aView,const Standard_ExtString aName,const Standard_CString aDomain = "") ;
Standard_EXPORT static  Handle_Aspect_WindowDriver WindowDriver(const Standard_CString aDisplay,const Standard_Integer ahiwin,const Standard_Integer alowin,const Quantity_NameOfColor aColor = Quantity_NOC_GRAY69) ;
Standard_EXPORT static  Handle_Aspect_WindowDriver XdpsDriver(const Standard_CString aDisplay,const Standard_Integer ahiwin,const Standard_Integer alowin,const Quantity_NameOfColor aColor = Quantity_NOC_GRAY69) ;
Standard_EXPORT static  Handle_Aspect_WindowDriver ImageDriver(const Standard_CString aDisplay,const Standard_Integer ahiwin,const Standard_Integer alowin,const Quantity_NameOfColor aColor = Quantity_NOC_GRAY69) ;


protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //


};





// other inline functions and methods (like "C++: function call" methods)
//


#endif

