using namespace std;
//  File      : SALOME_NumberFilter.cxx
//  Created   : Wed Feb 20 17:24:59 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOME_NumberFilter.ixx"
#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"

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
