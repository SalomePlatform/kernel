//  SALOME FILTER : interactive object for VISU entities implementation
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
//  File   : SALOME_PassThroughFilter.h
//  Author : Laurent CORNABE with help of Nicolas REJNERI
//  Module : SALOME


#ifndef SALOME_PassThroughFilter_HeaderFile
#define SALOME_PassThroughFilter_HeaderFile

#include <vtkDataSetToDataSetFilter.h>

class SALOME_PassThroughFilter : public vtkDataSetToDataSetFilter
{
public:
  vtkTypeRevisionMacro(SALOME_PassThroughFilter,vtkDataSetToDataSetFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Create a new SALOME_PassThroughFilter.
  static SALOME_PassThroughFilter *New();


protected:

  SALOME_PassThroughFilter() {};
  virtual ~SALOME_PassThroughFilter() {};

  void Execute();

private:
  SALOME_PassThroughFilter(const SALOME_PassThroughFilter&);  // Not implemented.
  void operator=(const SALOME_PassThroughFilter&);  // Not implemented.
};

#endif
