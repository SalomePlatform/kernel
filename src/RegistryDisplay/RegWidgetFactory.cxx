//  SALOME RegistryDisplay : GUI for Registry server implementation
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
//  File   : RegWidgetFactory.cxx
//  Author : Pascale NOYRET, EDF
//  Module : SALOME
//  $Header$

using namespace std;
# include "RegWidget.hxx"
# include "utilities.h"

extern "C"
{

void OnGUIEvent( CORBA::ORB_var &orb, QWidget *parent, const char *name )
{
  // N.B. parent can be 0
 RegWidget* ptrRegWidget = RegWidget::GetRegWidget( orb, parent, name );
 //ASSERT ( !ptrRegWidget );
 ptrRegWidget->show();
 ptrRegWidget->raise();
 ptrRegWidget->setActiveWindow();
 return;
}

}
