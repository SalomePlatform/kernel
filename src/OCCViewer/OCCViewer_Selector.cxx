using namespace std;
//  File      : OCCViewer_Selector.cxx
//  Created   : Wed Mar 20 10:57:47 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OCCViewer
//  Copyright : Open CASCADE 2002
//  $Header$


/*!
  \class OCCViewer_Selector OCCViewer_Selector.h
  \brief Base class for object selection in Open CASCADE Viewer.
*/

#include "OCCViewer_Selector.h"

/*!
    Constructor
*/
OCCViewer_Selector::OCCViewer_Selector() :
  myNumSelected (0),
  myEnableSelection( true ),
  myEnableMultipleSelection( true )
{
}

/*!
    Destructor
*/
OCCViewer_Selector::~OCCViewer_Selector()
{
}

/*!
    Enables/disables selection
*/
void OCCViewer_Selector::enableSelection( bool bEnable )
{
  myEnableSelection = bEnable;
}

/*!
    Enables/disables multiple selection i.e
    selection of several objects at the same time.
    If enabled, non-multiple selection is enabled as well.
*/
void OCCViewer_Selector::enableMultipleSelection( bool bEnable )
{
  myEnableMultipleSelection = bEnable;
  if ( bEnable ) myEnableSelection = bEnable;
}
