//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_SetValueDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_SetValueDlg.h"
#include "QAD_Tools.h"

#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvalidator.h>
using namespace std;

#define MARGIN_SIZE     11
#define SPACING_SIZE    6
/*!
  Constructor
*/
SALOMEGUI_SetValueDlg::SALOMEGUI_SetValueDlg( QWidget* parent )
    : QDialog( parent, "SALOMEGUI_SetValueDlg", true, 
	       WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  setCaption( tr("TLT_SETVALUE") );
  setSizeGripEnabled( TRUE );
  
  QVBoxLayout* topLayout = new QVBoxLayout( this );
  topLayout->setMargin( MARGIN_SIZE ); topLayout->setSpacing( SPACING_SIZE );

  /***************************************************************/
  QGroupBox* TopGroup = new QGroupBox( this, "TopGroup" );
  TopGroup->setColumnLayout(0, Qt::Vertical );
  TopGroup->layout()->setMargin( 0 ); TopGroup->layout()->setSpacing( 0 );
  QHBoxLayout* TopGroupLayout = new QHBoxLayout( TopGroup->layout() );
  TopGroupLayout->setAlignment( Qt::AlignTop );
  TopGroupLayout->setMargin( MARGIN_SIZE ); TopGroupLayout->setSpacing( SPACING_SIZE );
  
  myLabel = new QLabel( tr( "VALUE_LBL" ), TopGroup, "myLabel" );
  myLineEdit = new QLineEdit( TopGroup, "myLineEdit" );
  myLineEdit->setMinimumSize( 250, 0 );

  TopGroupLayout->addWidget( myLabel );
  TopGroupLayout->addWidget( myLineEdit );
  
  /***************************************************************/
  QGroupBox* GroupButtons = new QGroupBox( this, "GroupButtons" );
  GroupButtons->setColumnLayout(0, Qt::Vertical );
  GroupButtons->layout()->setMargin( 0 ); GroupButtons->layout()->setSpacing( 0 ); 
  QHBoxLayout* GroupButtonsLayout = new QHBoxLayout( GroupButtons->layout() );
  GroupButtonsLayout->setAlignment( Qt::AlignTop );
  GroupButtonsLayout->setMargin( MARGIN_SIZE ); GroupButtonsLayout->setSpacing( SPACING_SIZE );
  
  myButtonOk = new QPushButton( tr( "BUT_OK"  ), GroupButtons, "myButtonOk" );
  myButtonOk->setAutoDefault( TRUE ); myButtonOk->setDefault( TRUE );
  myButtonCancel = new QPushButton( tr( "BUT_CANCEL"  ), GroupButtons, "myButtonCancel" );
  myButtonCancel->setAutoDefault( TRUE );

  GroupButtonsLayout->addWidget( myButtonOk );
  GroupButtonsLayout->addStretch();
  GroupButtonsLayout->addWidget( myButtonCancel );
  /***************************************************************/
  
  topLayout->addWidget( TopGroup );
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
SALOMEGUI_SetValueDlg::~SALOMEGUI_SetValueDlg()
{
}

/*!
  Sets value
*/
void SALOMEGUI_SetValueDlg::setValue( const QString& value )
{
  myLineEdit->setText( value );
  myLineEdit->selectAll();
}

/*!
  Returns value entered by user
*/
QString SALOMEGUI_SetValueDlg::value()
{
  return myLineEdit->text();
}

/*!
  Sets validator
*/
void SALOMEGUI_SetValueDlg::setValidator( QValidator* v )
{
  myLineEdit->setValidator( v );
}

/*!
  Sets label text
*/
void SALOMEGUI_SetValueDlg::setLabel( const QString& label )
{
  myLabel->setText( label );
}

void SALOMEGUI_SetValueDlg::accept()
{
  const QValidator* v = myLineEdit->validator();
  if ( v ) {
    if ( v->inherits( "QIntValidator" ) ) {
      bool b;
      int val = myLineEdit->text().toInt( &b );
      const QIntValidator* iv = dynamic_cast<const QIntValidator*>(v);
      if ( !b || val < iv->bottom() || val > iv->top())
	return;
    }
  }
  QDialog::accept();
}

/*!
  Gets string value
*/
QString SALOMEGUI_SetValueDlg::getString ( const QString& caption, 
					   const QString& label, 
					   const QString& oldValue,
					   bool*          ok, 
					   QWidget*       parent )
{
  QString v = QString::null;
  SALOMEGUI_SetValueDlg* dlg = new SALOMEGUI_SetValueDlg( parent );
  dlg->setCaption( caption );
  dlg->setLabel( label );
  if ( !oldValue.isNull() )
    dlg->setValue( oldValue );
  int result = dlg->exec();
  if ( result == QDialog::Accepted ) 
    v = dlg->value();
  if ( ok ) 
    *ok = result == QDialog::Accepted;
  delete dlg;
  return v;
}

/*!
  Gets integer value
*/
int SALOMEGUI_SetValueDlg::getInteger( const QString& caption, 
				       const QString& label, 
				       const int      oldValue,
				       bool*          ok, 
				       QWidget*       parent )
{
  int v = 0;
  SALOMEGUI_SetValueDlg* dlg = new SALOMEGUI_SetValueDlg( parent );
  dlg->setCaption( caption );
  dlg->setLabel( label );
  dlg->setValidator( new QIntValidator( dlg ) );
  dlg->setValue( QString::number( oldValue ) );
  int result = dlg->exec();
  if ( result == QDialog::Accepted ) 
    v = dlg->value().toInt();
  if ( ok ) 
    *ok = result == QDialog::Accepted;
  delete dlg;
  return v;
}

/*!
  Gets integer value
*/
int SALOMEGUI_SetValueDlg::getInteger( const QString& caption, 
				       const QString& label, 
				       int            bottom,
				       int            top,
				       const int      oldValue,
				       bool*          ok, 
				       QWidget*       parent )
{
  int v = 0;
  SALOMEGUI_SetValueDlg* dlg = new SALOMEGUI_SetValueDlg( parent );
  dlg->setCaption( caption );
  dlg->setLabel( label );
  dlg->setValidator( new QIntValidator( bottom, top, dlg ) );
  dlg->setValue( QString::number( oldValue ) );
  int result = dlg->exec();
  if ( result == QDialog::Accepted ) 
    v = dlg->value().toInt();
  if ( ok ) 
    *ok = result == QDialog::Accepted;
  delete dlg;
  return v;
}

/*!
  Gets double value
*/
double SALOMEGUI_SetValueDlg::getDouble ( const QString& caption, 
					  const QString& label, 
					  const double   oldValue,
					  bool*          ok, 
					  QWidget*       parent )
{
  double v = 0;
  SALOMEGUI_SetValueDlg* dlg = new SALOMEGUI_SetValueDlg( parent );
  dlg->setCaption( caption );
  dlg->setLabel( label );
  dlg->setValidator( new QDoubleValidator( dlg ) );
  dlg->setValue( QString::number( oldValue ) );
  int result = dlg->exec();
  if ( result == QDialog::Accepted ) 
    v = dlg->value().toDouble();
  if ( ok ) 
    *ok = result == QDialog::Accepted;
  delete dlg;
  return v;
}
