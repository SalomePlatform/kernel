//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : SALOME_NumberFilter.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SALOME_NumberFilter.ixx"
#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
using namespace std;

SALOME_NumberFilter::SALOME_NumberFilter(NumberFilter TheKind, Standard_Integer aValue):
myKind(TheKind),
myValue(aValue){}

Standard_Boolean SALOME_NumberFilter::IsOk(const Handle(SALOME_InteractiveObject)& anObj) const 
{
  bool result = false;
  QAD_Study* myActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
  int nbSel = Sel->IObjectCount();

  switch ( myKind )
    {
    case 0 : // INFERIOR
      {
	if ( (nbSel + 1 ) < myValue )
	  result = true;
	else
	  result = false;
	break;
      }
    case 1 : // INFERIOR_OR_EQUAL
      {
	if ( (nbSel + 1 ) <= myValue )
	  result = true;
	else
	  result = false;
	break;
      }
    case 2 : // SUPERIOR
      {
	result = true;
	break;
      }
    case 3 : // SUPERIOR_OR_EQUAL
      {
	result = true;
	break;
      }
    case 4 : // EQUAL
      {
	if ( (nbSel + 1 ) == myValue )
	  result = true;
	else
	  result = false;
	break;
      }
    }

  return result;
}
