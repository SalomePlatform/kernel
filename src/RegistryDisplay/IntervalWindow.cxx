//  SALOME RegistryDisplay : GUI for Registry server implementation
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : IntervalWindow.cxx
//  Author : Oksana TCHEBANOVA
//  Module : SALOME

#include <IntervalWindow.hxx>

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
# include "utilities.h"
using namespace std;

#define MARGIN_SIZE      11
#define SPACING_SIZE      6
#define MIN_SPIN_WIDTH  100 

IntervalWindow::IntervalWindow ( QWidget* parent )
     : QDialog( parent, "IntervalWindow" , true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu | WDestructiveClose )
{
  BEGIN_OF("IntervalWindow constructor")
  setCaption( tr( "Refresh Interval"  ) );
  setSizeGripEnabled( true );

  QGridLayout* topLayout = new QGridLayout( this );
  topLayout->setSpacing( SPACING_SIZE );
  topLayout->setMargin( MARGIN_SIZE );

  QGroupBox* intervalGrp = new QGroupBox( this, "intervalGrp" );
  intervalGrp->setColumnLayout( 0, Qt::Horizontal );//Vertical
  intervalGrp->layout()->setSpacing( 0 );
  intervalGrp->layout()->setMargin( 0 );
  QGridLayout* intervalGrpLayout = new QGridLayout( intervalGrp->layout() );
  intervalGrpLayout->setAlignment( Qt::AlignTop );
  intervalGrpLayout->setSpacing( SPACING_SIZE );
  intervalGrpLayout->setMargin( MARGIN_SIZE  );  

  QHBoxLayout* aBtnLayout = new QHBoxLayout;
  aBtnLayout->setSpacing( SPACING_SIZE );
  aBtnLayout->setMargin( 0 );

  myButtonOk = new QPushButton( this, "buttonOk" );
  myButtonOk->setText( tr( "BUT_OK"  ) );
  myButtonOk->setAutoDefault( TRUE );
  myButtonOk->setDefault( TRUE );
  
  myButtonCancel = new QPushButton( this, "buttonCancel" );
  myButtonCancel->setText( tr( "BUT_CANCEL"  ) );
  myButtonCancel->setAutoDefault( TRUE );

  QLabel* TextLabel = new QLabel( intervalGrp, "TextLabel" );
  TextLabel->setText( tr( "Please, enter a number of seconds:"  ) );

  mySpinBox = new QSpinBox( 1, 999999999, 1, intervalGrp, "SpinBox" );
  mySpinBox->setValue( 100 );
  mySpinBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
  mySpinBox->setMinimumWidth(MIN_SPIN_WIDTH);

  intervalGrpLayout->addWidget(TextLabel, 0, 0);
  intervalGrpLayout->addWidget(mySpinBox, 0, 1);

  aBtnLayout->addWidget( myButtonOk );
  aBtnLayout->addItem( new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
  aBtnLayout->addWidget( myButtonCancel );

  topLayout->addWidget( intervalGrp, 0, 0 );
  topLayout->addLayout( aBtnLayout, 1, 0 ); 

  END_OF("IntervalWindow constructor")
}
IntervalWindow::~IntervalWindow() {}
/* 
   Sets start interval size
*/
void IntervalWindow::setValue(const int size)
{
  mySpinBox->setValue(size);
}

/*
   Gets interval size
*/
int IntervalWindow::getValue()
{
  return mySpinBox->value();
}

/*
   gets a pointer to myButtonOk
*/
QPushButton* IntervalWindow::Ok()
{
  return myButtonOk;
}

/*
   gets a pointer to myButtonCancel
*/
QPushButton* IntervalWindow::Cancel()
{
  return myButtonCancel;
}
