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
//  File   : SALOMEGUI_TrihedronSizeDlg.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_TrihedronSizeDlg.h"

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
using namespace std;

#define MARGIN_SIZE      11
#define SPACING_SIZE      6
#define MIN_SPIN_WIDTH  100 

/* 
 *  Constructs a modal SALOMEGUI_TrihedronSizeDlg which is a child of 'parent'
 *
 */
SALOMEGUI_TrihedronSizeDlg::SALOMEGUI_TrihedronSizeDlg( QWidget* parent )
     : QDialog( parent, "SALOMEGUI_TrihedronSizeDlg", true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  setCaption( tr( "MEN_TRIHEDRON"  ) );
  setSizeGripEnabled( true );

  QGridLayout* topLayout = new QGridLayout( this );
  topLayout->setSpacing( SPACING_SIZE );
  topLayout->setMargin( MARGIN_SIZE );

  QGroupBox* trihedronGrp = new QGroupBox( this, "trihedronGrp" );
  trihedronGrp->setColumnLayout( 0, Qt::Vertical );
  trihedronGrp->layout()->setSpacing( 0 );
  trihedronGrp->layout()->setMargin( 0 );
  QGridLayout* trihedronGrpLayout = new QGridLayout( trihedronGrp->layout() );
  trihedronGrpLayout->setAlignment( Qt::AlignTop );
  trihedronGrpLayout->setSpacing( SPACING_SIZE );
  trihedronGrpLayout->setMargin( MARGIN_SIZE  );  

  QHBoxLayout* aBtnLayout = new QHBoxLayout;
  aBtnLayout->setSpacing( SPACING_SIZE );
  aBtnLayout->setMargin( 0 );

  QPushButton* buttonOk = new QPushButton( this, "buttonOk" );
  buttonOk->setText( tr( "BUT_OK"  ) );
  buttonOk->setAutoDefault( TRUE );
  buttonOk->setDefault( TRUE );
  
  QPushButton* buttonCancel = new QPushButton( this, "buttonCancel" );
  buttonCancel->setText( tr( "BUT_CANCEL"  ) );
  buttonCancel->setAutoDefault( TRUE );

  QLabel* TextLabel = new QLabel( trihedronGrp, "TextLabel" );
  TextLabel->setText( tr( "MEN_TRIHEDRON_SIZE"  ) );

  SpinBox = new QSpinBox( 1, 999999999, 1, trihedronGrp, "SpinBox" );
  SpinBox->setValue( 100 );
  SpinBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
  SpinBox->setMinimumWidth(MIN_SPIN_WIDTH);

  trihedronGrpLayout->addWidget(TextLabel, 0, 0);
  trihedronGrpLayout->addWidget(SpinBox, 0, 1);

  aBtnLayout->addWidget( buttonOk );
  aBtnLayout->addItem( new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
  aBtnLayout->addWidget( buttonCancel );

  topLayout->addWidget( trihedronGrp, 0, 0 );
  topLayout->addLayout( aBtnLayout, 1, 0 ); 

  // signals and slots connections
  connect( buttonOk,     SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
SALOMEGUI_TrihedronSizeDlg::~SALOMEGUI_TrihedronSizeDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
   Sets start trihedron size
*/
void SALOMEGUI_TrihedronSizeDlg::setTrihedronSize(const int size)
{
  SpinBox->setValue(size);
}

/*
   Gets trihedron size
*/
int SALOMEGUI_TrihedronSizeDlg::getTrihedronSize()
{
  return SpinBox->value();
}
