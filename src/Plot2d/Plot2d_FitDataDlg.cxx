//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_FitDataDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "Plot2d_FitDataDlg.h"
#include <qlabel.h>
#include <qlayout.h>
#include <qvalidator.h>
#include "QAD_Tools.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
using namespace std;

#define SPACING_SIZE      6
#define MARGIN_SIZE       11
#define MIN_EDIT_SIZE     100

/*!
  Constructor 
*/
Plot2d_FitDataDlg::Plot2d_FitDataDlg( QWidget* parent )
     : QDialog( parent ? parent : QAD_Application::getDesktop(), 
	       "Plot2d_FitDataDlg", 
	       true, 
	       WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  setCaption( tr( "FIT_DATA_TLT" ) );
  setSizeGripEnabled( TRUE );
  QGridLayout* topLayout = new QGridLayout( this ); 
  topLayout->setSpacing( SPACING_SIZE );
  topLayout->setMargin( MARGIN_SIZE );

  // 'Range' group
  myRangeGrp = new QButtonGroup( this );
  myRangeGrp->setColumnLayout( 0, Qt::Vertical );
  myRangeGrp->layout()->setSpacing( 0 );
  myRangeGrp->layout()->setMargin( 0 );
  QGridLayout* aGridLayout = new QGridLayout( myRangeGrp->layout() );
  aGridLayout->setAlignment( Qt::AlignTop );
  aGridLayout->setMargin( MARGIN_SIZE );
  aGridLayout->setSpacing( SPACING_SIZE );

  myModeAllRB  = new QRadioButton( tr( "FIT_ALL" ),        myRangeGrp );
  myModeHorRB  = new QRadioButton( tr( "FIT_HORIZONTAL" ), myRangeGrp );
  myModeVerRB  = new QRadioButton( tr( "FIT_VERTICAL" ),   myRangeGrp );

  QDoubleValidator* aValidator = new QDoubleValidator( this );
  myXMinEdit = new QLineEdit( myRangeGrp );
  myXMinEdit->setValidator( aValidator );
  myXMinEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myXMinEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
  myXMinEdit->setText( "0.0" );

  myYMinEdit = new QLineEdit( myRangeGrp );
  myYMinEdit->setValidator( aValidator );
  myYMinEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myYMinEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
  myYMinEdit->setText( "0.0" );

  myXMaxEdit = new QLineEdit( myRangeGrp );
  myXMaxEdit->setValidator( aValidator );
  myXMaxEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myXMaxEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
  myXMaxEdit->setText( "0.0" );

  myYMaxEdit = new QLineEdit( myRangeGrp );
  myYMaxEdit->setValidator( aValidator );
  myYMaxEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myYMaxEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
  myYMaxEdit->setText( "0.0" );

  QFrame* aHLine = new QFrame( myRangeGrp );
  aHLine->setFrameStyle( QFrame::HLine | QFrame::Sunken );

  QHBoxLayout* aModeLayout = new QHBoxLayout;
  aModeLayout->setMargin( 0 );
  aModeLayout->setSpacing( SPACING_SIZE );
  aModeLayout->addWidget( myModeAllRB );
  aModeLayout->addWidget( myModeHorRB );
  aModeLayout->addWidget( myModeVerRB );

  QLabel* horLab = new QLabel( tr( "HORIZONTAL_AXIS" ), myRangeGrp );
  QLabel* verLab = new QLabel( tr( "VERTICAL_AXIS" ), myRangeGrp );
  QFont font = horLab->font(); font.setBold( true );
  horLab->setFont( font ); verLab->setFont( font );

  aGridLayout->addMultiCellLayout( aModeLayout,    0, 0, 0, 4 );
  aGridLayout->addMultiCellWidget( aHLine,         1, 1, 0, 4 );
  aGridLayout->addWidget         ( horLab,         2,    0 );
  aGridLayout->addWidget         ( new QLabel( tr( "MIN_VALUE_LAB" ), myRangeGrp ), 
                                                   2,    1 );
  aGridLayout->addWidget         ( myXMinEdit,     2,    2 );
  aGridLayout->addWidget         ( new QLabel( tr( "MAX_VALUE_LAB" ), myRangeGrp ), 
                                                   2,    3 );
  aGridLayout->addWidget         ( myXMaxEdit,     2,    4 );
  aGridLayout->addWidget         ( verLab,         3,    0 );
  aGridLayout->addWidget         ( new QLabel( tr( "MIN_VALUE_LAB" ), myRangeGrp ), 
                                                   3,    1 );
  aGridLayout->addWidget         ( myYMinEdit,     3,    2 );
  aGridLayout->addWidget         ( new QLabel( tr( "MAX_VALUE_LAB" ), myRangeGrp ), 
                                                   3,    3 );
  aGridLayout->addWidget         ( myYMaxEdit,     3,    4 );
  
  // OK/Cancel buttons
  myOkBtn = new QPushButton( tr( "BUT_OK" ), this, "buttonOk" );
  myOkBtn->setAutoDefault( TRUE );
  myOkBtn->setDefault( TRUE );
  myCancelBtn = new QPushButton( tr( "BUT_CANCEL" ), this, "buttonCancel" );
  myCancelBtn->setAutoDefault( TRUE );

  topLayout->addMultiCellWidget( myRangeGrp, 0, 0, 0, 2 );
  topLayout->addWidget( myOkBtn, 1, 0 );
  topLayout->setColStretch( 1, 5 );
  topLayout->addWidget( myCancelBtn, 1, 2 );

  // connect signals
  connect( myOkBtn,      SIGNAL( clicked() ),      this, SLOT( accept() ) );
  connect( myCancelBtn,  SIGNAL( clicked() ),      this, SLOT( reject() ) );
  connect( myRangeGrp,   SIGNAL( clicked( int ) ), this, SLOT( onModeChanged( int ) ) );

  // initial state
  myModeAllRB->setChecked( true );
  onModeChanged( 0 );

  /* Center widget inside it's parent widget */
  QAD_Tools::centerWidget( this, parentWidget() );
}

/*!
  Sets range
*/
void Plot2d_FitDataDlg::setRange( const double xMin, 
                                  const double xMax,
                                  const double yMin,
                                  const double yMax ) 
{
  myXMinEdit->setText( QString::number( xMin ) );
  myXMaxEdit->setText( QString::number( xMax ) );
  myYMinEdit->setText( QString::number( yMin ) );
  myYMaxEdit->setText( QString::number( yMax ) );
}

/*!
  Gets range, returns mode (see getMode())
*/
int Plot2d_FitDataDlg::getRange( double& xMin, 
                                 double& xMax,
                                 double& yMin,
                                 double& yMax ) 
{
  xMin = myXMinEdit->text().toDouble();
  xMax = myXMaxEdit->text().toDouble();
  yMin = myYMinEdit->text().toDouble();
  yMax = myYMaxEdit->text().toDouble();
  int myMode = 0;
  if ( myModeAllRB->isChecked() )
    myMode = 0;
  if ( myModeHorRB->isChecked() )
    myMode = 1;
  if ( myModeVerRB->isChecked() )
    myMode = 2;
  return myMode;
}

/*!
  Gets mode : 0 - Fit all; 1 - Fit horizontal, 2 - Fit vertical
*/
int Plot2d_FitDataDlg::getMode() 
{
  int myMode = 0;
  if ( myModeAllRB->isChecked() )
    myMode = 0;
  if ( myModeHorRB->isChecked() )
    myMode = 1;
  if ( myModeVerRB->isChecked() )
    myMode = 2;
  return myMode;
}

/*!
  Called when range mode changed
*/
void Plot2d_FitDataDlg::onModeChanged(int mode) 
{
  bool badFocus;
  switch( mode ) {
  case 0: // fit all mode
    myXMinEdit->setEnabled(true);
    myXMaxEdit->setEnabled(true);
    myYMinEdit->setEnabled(true);
    myYMaxEdit->setEnabled(true);
    break;
  case 1: // fit horizontal mode
    badFocus = myYMinEdit->hasFocus() || myYMaxEdit->hasFocus();
    myXMinEdit->setEnabled(true);
    myXMaxEdit->setEnabled(true);
    myYMinEdit->setEnabled(false);
    myYMaxEdit->setEnabled(false);
    if (badFocus)
      myXMinEdit->setFocus();
    break;
  case 2: // fit vertical mode
    badFocus = myXMinEdit->hasFocus() || myXMaxEdit->hasFocus();
    myXMinEdit->setEnabled(false);
    myXMaxEdit->setEnabled(false);
    myYMinEdit->setEnabled(true);
    myYMaxEdit->setEnabled(true);
    if (badFocus)
      myYMinEdit->setFocus();
    break;
  }
}

