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
//  File   : OCCViewer_AISSelector.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class OCCViewer_AISSelector OCCViewer_AISSelector.h
  \brief Selector for AIS interactive context.

  Used only by Viewer Open CASCADE.
*/

#include "OCCViewer_AISSelector.h"
using namespace std;

/*!
    Constructor
*/
OCCViewer_AISSelector::OCCViewer_AISSelector( const Handle (AIS_InteractiveContext)& aisContext) :
myAISContext ( aisContext )
{
  initialize();
}

/*!
    Destructor
*/
OCCViewer_AISSelector::~OCCViewer_AISSelector()
{
  cleanup();
}

/*!
    Initialization ( internal )
*/
void OCCViewer_AISSelector::initialize()
{
  QAD_ASSERT_DEBUG_ONLY ( !myAISContext.IsNull() );

  myHilightColor = Quantity_NOC_CYAN1;
  mySelectColor = Quantity_NOC_GRAY80;

  myAISContext->SetHilightColor( myHilightColor );
  myAISContext->SelectionColor( mySelectColor );
  myAISContext->SetSubIntensityColor( Quantity_NOC_CYAN1 );
}

/*!
    Cleanup ( internal )
*/
void OCCViewer_AISSelector::cleanup()
{
}

/*!
    Sets the color to hilight the detected objects
*/
void OCCViewer_AISSelector::setHilightColor ( Quantity_NameOfColor color )
{
  myHilightColor = color;
}

/*!
    Sets the color to display the selected objects
*/
void OCCViewer_AISSelector::setSelectColor ( Quantity_NameOfColor color )
{
  mySelectColor = color;
}

/*!
  Sets the interactive context for this selector
*/
void OCCViewer_AISSelector::setContext ( const Handle (AIS_InteractiveContext)& aisContext )
{
  QAD_ASSERT_DEBUG_ONLY ( !aisContext.IsNull() );
  myAISContext = aisContext;
}

/*!
    Checks the status of pick and emits 'selSelectionDone' or
    'selSelectionCancel'.
    Returns 'true' if no error, 'false' otherwise.
*/
bool OCCViewer_AISSelector::checkSelection ( AIS_StatusOfPick status, bool hadSelection, bool addTo )
{
  myNumSelected = myAISContext->NbCurrents(); /* update after the last selection */

  if ( status == AIS_SOP_NothingSelected && !hadSelection ) {
    emit selSelectionCancel( addTo );
  }
  else if ( status == AIS_SOP_NothingSelected && hadSelection ) {
    emit selSelectionCancel( addTo ); /* unselected now */
  }
  else if ( status == AIS_SOP_OneSelected || status == AIS_SOP_SeveralSelected )
    {
      //      if ( !hadSelection || status != AIS_SOP_SeveralSelected )
      //	addTo = false;
      //      MESSAGE (" AIS_StatusOfPick : " << AIS_SOP_OneSelected )

      emit selSelectionDone( addTo ); /* selected ( the same object, may be ) */
    }
  return ( status != AIS_SOP_Error && status != AIS_SOP_NothingSelected );
}


/*!
    Detects the interactive objects at position (x,y).
    Returns 'true' if no error, 'false' otherwise.
*/
bool OCCViewer_AISSelector::moveTo ( int x, int y, const Handle (V3d_View)& view )
{
  if ( !myEnableSelection )
    return false;

  QAD_ASSERT_DEBUG_ONLY ( !myAISContext.IsNull() );
  AIS_StatusOfDetection status = AIS_SOD_Error;
  status = myAISContext->MoveTo (x, y, view);

  return ( status != AIS_SOD_Error && status != AIS_SOD_AllBad );
}

/*!
    Selects the detected interactive objects.
    Calls checkSelection() for checking the status.
*/
bool OCCViewer_AISSelector::select ()
{
  if ( !myEnableSelection )
    return false;

  QAD_ASSERT_DEBUG_ONLY ( !myAISContext.IsNull() );
  bool hadSelection = ( myNumSelected > 0 );

  /* select and send notifications */
  return checkSelection ( myAISContext->Select(), hadSelection, false );
}

/*!
    Selects the objects covered by the rectangle.
    Multiple selection must be enabled to get use of this function.
    Calls checkSelection() for checking the status.
*/
bool OCCViewer_AISSelector::select ( int left, int top, int right, int bottom,
			       const Handle (V3d_View)& view )
{
  if ( !myEnableSelection || !myEnableMultipleSelection )
    return false;	/* selection with rectangle is considered as multiple selection */

  QAD_ASSERT_DEBUG_ONLY ( !myAISContext.IsNull() );
  bool hadSelection = ( myNumSelected > 0 );

  /* select and send notifications */
  return checkSelection ( myAISContext->Select(left, top, right, bottom, view),
			  hadSelection, false );
}

/*!
    Adds new selected objects to the objects previously selected.
    Multiple selection must be enabled to get use of this function.
    Calls checkSelection() for checking the status.
*/
bool OCCViewer_AISSelector::shiftSelect ()
{
  if ( !myEnableSelection )
    return false;

  QAD_ASSERT_DEBUG_ONLY ( !myAISContext.IsNull() );
  bool hadSelection = ( myNumSelected > 0 ); /* something was selected */
  if ( hadSelection && !myEnableMultipleSelection)
    return false;

  /* select and send notifications */
  return checkSelection ( myAISContext->ShiftSelect(), hadSelection, true );
}

/*!
    Adds new selected objects covered by the rectangle to the objects
    previously selected.
    Multiple selection must be enabled to get use of this function.
    Calls checkSelection() for checking the status.
*/
bool OCCViewer_AISSelector::shiftSelect ( int left, int top, int right, int bottom,
				    const Handle (V3d_View)& view )

{
  if ( !myEnableSelection || !myEnableMultipleSelection )
    return false;	/* selection with rectangle is considered as multiple selection */

  QAD_ASSERT_DEBUG_ONLY ( !myAISContext.IsNull() );
  bool hadSelection = ( myNumSelected > 0 );			/* something was selected */
  if ( hadSelection && !myEnableMultipleSelection)
    return false;

  /* select and send notifications */
  return checkSelection ( myAISContext->ShiftSelect(left,top,right,bottom, view),
			  hadSelection, true );
}
