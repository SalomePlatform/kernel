//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_NameDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_NameDlg.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Tools.h"

#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
using namespace std;

/*!
  Constructor
*/
SALOMEGUI_NameDlg::SALOMEGUI_NameDlg( QWidget* parent )
    : QDialog( parent ? parent : QAD_Application::getDesktop(), 
	       "SALOMEGUI_NameDlg", 
	       true, 
	       WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  setCaption( tr("TLT_RENAME") );
  setSizeGripEnabled( TRUE );
  
  QVBoxLayout* topLayout = new QVBoxLayout( this );
  topLayout->setMargin( 11 ); topLayout->setSpacing( 6 );

  /***************************************************************/
  QGroupBox* GroupC1 = new QGroupBox( this, "GroupC1" );
  GroupC1->setColumnLayout(0, Qt::Vertical );
  GroupC1->layout()->setMargin( 0 ); GroupC1->layout()->setSpacing( 0 );
  QHBoxLayout* GroupC1Layout = new QHBoxLayout( GroupC1->layout() );
  GroupC1Layout->setAlignment( Qt::AlignTop );
  GroupC1Layout->setMargin( 11 ); GroupC1Layout->setSpacing( 6 );
  
  QLabel* TextLabel = new QLabel( GroupC1, "TextLabel1" );
  TextLabel->setText( tr( "NAME_LBL" ) );
  GroupC1Layout->addWidget( TextLabel );
  
  myLineEdit = new QLineEdit( GroupC1, "LineEdit1" );
  myLineEdit->setMinimumSize( 250, 0 );
  GroupC1Layout->addWidget( myLineEdit );
  
  /***************************************************************/
  QGroupBox* GroupButtons = new QGroupBox( this, "GroupButtons" );
  GroupButtons->setColumnLayout(0, Qt::Vertical );
  GroupButtons->layout()->setMargin( 0 ); GroupButtons->layout()->setSpacing( 0 ); 
  QHBoxLayout* GroupButtonsLayout = new QHBoxLayout( GroupButtons->layout() );
  GroupButtonsLayout->setAlignment( Qt::AlignTop );
  GroupButtonsLayout->setMargin( 11 ); GroupButtonsLayout->setSpacing( 6 );
  
  myButtonOk = new QPushButton( GroupButtons, "buttonOk" );
  myButtonOk->setText( tr( "BUT_OK"  ) );
  myButtonOk->setAutoDefault( TRUE ); myButtonOk->setDefault( TRUE );
  GroupButtonsLayout->addWidget( myButtonOk );

  GroupButtonsLayout->addStretch();
  
  myButtonCancel = new QPushButton( GroupButtons, "buttonCancel" );
  myButtonCancel->setText( tr( "BUT_CANCEL"  ) );
  myButtonCancel->setAutoDefault( TRUE );
  GroupButtonsLayout->addWidget( myButtonCancel );
  /***************************************************************/
  
  topLayout->addWidget( GroupC1 );
  topLayout->addWidget( GroupButtons );
  
  // signals and slots connections
  connect( myButtonOk,     SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( myButtonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
  
  /* Move widget on the botton right corner of main widget */
  QAD_Tools::centerWidget( this, parent );
}

/*!
  Destructor
*/
SALOMEGUI_NameDlg::~SALOMEGUI_NameDlg()
{
}

/*!
  Sets name
*/
void SALOMEGUI_NameDlg::setName( const QString& name )
{
  myLineEdit->setText( name );
  myLineEdit->selectAll();
}

/*!
  Returns name entered by user
*/
QString SALOMEGUI_NameDlg::name()
{
  return myLineEdit->text();
}

/*!
  Creates modal <Rename> dialog and returns name entered [ static ]
*/
QString SALOMEGUI_NameDlg::getName( QWidget* parent, const QString& oldName )
{
  QString n;
  SALOMEGUI_NameDlg* dlg = new SALOMEGUI_NameDlg( parent );
  if ( !oldName.isNull() )
    dlg->setName( oldName );
  if ( dlg->exec() == QDialog::Accepted ) 
    n = dlg->name();
  delete dlg;
  return n;
}
