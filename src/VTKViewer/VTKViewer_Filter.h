//  SMESHGUI_Filter : Filter for VTK viewer
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
//  File   : VTKViewer_Filter.h
//  Author : Sergey LITONIN
//  Module : SALOME

#ifndef VTKViewer_Filter_HeaderFile
#define VTKViewer_Filter_HeaderFile

#include <MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>

class SALOME_Actor;

DEFINE_STANDARD_HANDLE(VTKViewer_Filter, MMgt_TShared)

/*
  Class       : VTKViewer_Filter
  Description : Base class of filters of for VTK viewer. Method IsValid 
                should be redefined in derived classes
*/

class VTKViewer_Filter : public MMgt_TShared
{

public:
                              VTKViewer_Filter();
  virtual                     ~VTKViewer_Filter();

  bool                        IsValid( SALOME_Actor*, const int theCellId );
  virtual bool                IsValid( const int theCellId ) const = 0;

  virtual void                SetActor( SALOME_Actor* );

protected:

  SALOME_Actor*               myActor;

public:  
  DEFINE_STANDARD_RTTI(VTKViewer_Filter)
};

#endif
