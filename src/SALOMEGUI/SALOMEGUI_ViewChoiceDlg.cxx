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
//  File   : SALOMEGUI_ViewChoiceDlg.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_ViewChoiceDlg.h"
#include "QAD_Config.h"
#include "QAD_StudyFrame.h"

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
using namespace std;

/* 
 *  Constructs a SALOMEGUI_ViewChoiceDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SALOMEGUI_ViewChoiceDlg::SALOMEGUI_ViewChoiceDlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
    if ( !name )
	setName( "SALOMEGUI_ViewChoiceDlg" );
    setCaption( tr( "MEN_CHOICE" ) );
    setSizeGripEnabled( true );

    QGridLayout* theBaseLayout = new QGridLayout( this, 2, 1, 11, 6);

    GroupBox1 = new QGroupBox(0, Qt::Vertical, tr("MEN_VIEW"), this);
    theBaseLayout->addWidget(GroupBox1, 0, 0);

    QHBoxLayout* aBoxLayout = new QHBoxLayout(GroupBox1->layout()); 

    QHBoxLayout* aInternalLayout = new QHBoxLayout(6);

    TextLabel1 = new QLabel(tr("MEN_VIEWER_TYPE"), GroupBox1);
    aInternalLayout->addWidget(TextLabel1);

    ComboBox1 = new QComboBox( false, GroupBox1);
    ComboBox1->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    ComboBox1->setMinimumWidth( 200 );
    // OCC Viewer
    ComboBox1->insertItem(tr("MEN_VIEWER_OCC"));
    myViewers.append(VIEW_OCC);
    if ( QAD_CONFIG->getSetting( "Viewer:DefaultViewer").toInt() == VIEW_OCC )
      ComboBox1->setCurrentItem( ComboBox1->count()-1 );
    // VTK Viewer
    ComboBox1->insertItem(tr("MEN_VIEWER_VTK"));
    myViewers.append(VIEW_VTK);
    if ( QAD_CONFIG->getSetting( "Viewer:DefaultViewer").toInt() == VIEW_VTK )
      ComboBox1->setCurrentItem( ComboBox1->count()-1 );
    // SUPERVISOR Viewer
//    ComboBox1->insertItem(tr("MEN_VIEWER_GRAPH"));
//    myViewers.append(VIEW_GRAPHSUPERV);
//    if ( QAD_CONFIG->getSetting( "Viewer:DefaultViewer").toInt() == VIEW_GRAPHSUPERV )
//      ComboBox1->setCurrentItem( ComboBox1->count()-1 );
    // Plot2d Viewer
    ComboBox1->insertItem(tr("MEN_VIEWER_PLOT2D"));
    myViewers.append(VIEW_PLOT2D);
    if ( QAD_CONFIG->getSetting( "Viewer:DefaultViewer").toInt() == VIEW_PLOT2D )
      ComboBox1->setCurrentItem( ComboBox1->count()-1 );
    TextLabel1->setBuddy(ComboBox1);
    aInternalLayout->addWidget(ComboBox1);

    aBoxLayout->addLayout(aInternalLayout);
    
    QFrame* aButtonFrame = new QFrame( this );
    theBaseLayout->addWidget(aButtonFrame, 1, 0);

    QHBoxLayout* aButtLayout = new QHBoxLayout(aButtonFrame); 

    buttonOk = new QPushButton(tr("BUT_OK"), aButtonFrame);
    buttonOk->setAutoDefault(true);
    buttonOk->setDefault(true);
    aButtLayout->addWidget(buttonOk);

    QSpacerItem* aSpacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding);
    aButtLayout->addItem(aSpacer);

    buttonCancel = new QPushButton(tr( "BUT_CANCEL" ),  aButtonFrame);
    buttonCancel->setAutoDefault(true);
    aButtLayout->addWidget(buttonCancel);

    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
SALOMEGUI_ViewChoiceDlg::~SALOMEGUI_ViewChoiceDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*!
  Gets user's choice
*/
int SALOMEGUI_ViewChoiceDlg::getSelectedViewer()
{
  return myViewers[ ComboBox1->currentItem() ];
}
