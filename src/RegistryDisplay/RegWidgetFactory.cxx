using namespace std;
//=============================================================================
// File      : RegWidgetFactory.cxx
// Created   : Tue Nov 13 17:16:01 CET 2001
// Author    : Pascale NOYRET, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

# include "RegWidget.hxx"
# include "utilities.h"

extern "C"
{

void OnGUIEvent( CORBA::ORB_var &orb, QWidget *parent, const char *name )
{
  // N.B. parent can be 0
 RegWidget* ptrRegWidget = RegWidget::GetRegWidget( orb, parent, name );
 ASSERT ( !ptrRegWidget );
 ptrRegWidget->show();
 ptrRegWidget->raise();
 ptrRegWidget->setActiveWindow();
 return;
}

}
