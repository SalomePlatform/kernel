//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_SetupCurveDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_SetupCurveDlg.h"
#include "QAD_Tools.h"
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qtoolbutton.h>
#include <qgroupbox.h>
#include <qcolordialog.h>
using namespace std;

#define MARGIN_SIZE      11
#define SPACING_SIZE     6
#define MIN_COMBO_WIDTH  100
#define MIN_SPIN_WIDTH   50
#define MAX_LINE_WIDTH   100

/*!
  Constructor
*/
SALOMEGUI_SetupCurveDlg::SALOMEGUI_SetupCurveDlg( QWidget* parent )
     : QDialog( parent, "SALOMEGUI_SetupCurveDlg", true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  setCaption( tr("TLT_SETUP_CURVE") );
  setSizeGripEnabled( TRUE );
  QGridLayout* topLayout = new QGridLayout( this ); 
  topLayout->setSpacing( SPACING_SIZE );
  topLayout->setMargin( MARGIN_SIZE );

  QGroupBox* TopGroup = new QGroupBox( this );
  TopGroup->setColumnLayout( 0, Qt::Vertical );
  TopGroup->layout()->setSpacing( 0 ); TopGroup->layout()->setMargin( 0 );
  QGridLayout* TopGroupLayout = new QGridLayout( TopGroup->layout() );
  TopGroupLayout->setAlignment( Qt::AlignTop );
  TopGroupLayout->setSpacing( SPACING_SIZE ); TopGroupLayout->setMargin( MARGIN_SIZE );

  QLabel* aLineTypeLab = new QLabel( tr( "CURVE_LINE_TYPE_LAB" ), TopGroup );
  myLineCombo = new QComboBox( false, TopGroup );
  myLineCombo->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myLineCombo->setMinimumWidth( MIN_COMBO_WIDTH );
  myLineCombo->insertItem( tr( "NONE_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "SOLID_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DASH_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DOT_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DASHDOT_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DAHSDOTDOT_LINE_LBL" ) );
  myLineCombo->setCurrentItem( 1 ); // SOLID by default

  QLabel* aLineWidthLab = new QLabel( tr( "CURVE_LINE_WIDTH_LAB" ), TopGroup );
  myLineSpin = new QSpinBox( 0, MAX_LINE_WIDTH, 1, TopGroup );
  myLineSpin->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myLineSpin->setMinimumWidth( MIN_SPIN_WIDTH );
  myLineSpin->setValue( 0 );        // default width is 0

  QLabel* aMarkerLab = new QLabel( tr( "CURVE_MARKER_TYPE_LAB" ), TopGroup );
  myMarkerCombo = new QComboBox( false, TopGroup );
  myMarkerCombo->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myMarkerCombo->setMinimumWidth( MIN_COMBO_WIDTH );
  myMarkerCombo->insertItem( tr( "NONE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "CIRCLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "RECTANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "DIAMOND_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "DTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "UTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "LTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "RTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "CROSS_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "XCROSS_MARKER_LBL" ) );
  myMarkerCombo->setCurrentItem( 1 ); // CIRCLE by default

  QLabel* aColorLab = new QLabel( tr( "CURVE_COLOR_LAB" ), TopGroup );
  myColorBtn = new QToolButton( TopGroup );

  TopGroupLayout->addWidget( aLineTypeLab, 0, 0 );
  TopGroupLayout->addMultiCellWidget( myLineCombo, 0, 0, 1, 2 );
  TopGroupLayout->addWidget( aLineWidthLab, 1, 0 );
  TopGroupLayout->addMultiCellWidget( myLineSpin, 1, 1, 1, 2 );
  TopGroupLayout->addWidget( aMarkerLab, 2, 0 );
  TopGroupLayout->addMultiCellWidget( myMarkerCombo, 2, 2, 1, 2 );
  TopGroupLayout->addWidget( aColorLab, 3, 0 );
  TopGroupLayout->addWidget( myColorBtn, 3, 1 );
  TopGroupLayout->setColStretch( 2, 5 );

  QGroupBox* GroupButtons = new QGroupBox( this );
  GroupButtons->setColumnLayout( 0, Qt::Vertical );
  GroupButtons->layout()->setSpacing( 0 ); GroupButtons->layout()->setMargin( 0 );
  QHBoxLayout* GroupButtonsLayout = new QHBoxLayout( GroupButtons->layout() );
  GroupButtonsLayout->setAlignment( Qt::AlignTop );
  GroupButtonsLayout->setSpacing( SPACING_SIZE ); GroupButtonsLayout->setMargin( MARGIN_SIZE );

  myOkBtn = new QPushButton( tr( "BUT_OK" ), GroupButtons );
  myOkBtn->setAutoDefault( true ); myOkBtn->setDefault( true );
  myCancelBtn = new QPushButton( tr( "BUT_CANCEL" ) , GroupButtons );
  myCancelBtn->setAutoDefault( true );

  GroupButtonsLayout->addWidget( myOkBtn );
  GroupButtonsLayout->addStretch();
  GroupButtonsLayout->addWidget( myCancelBtn );

  connect( myColorBtn,  SIGNAL( clicked() ), this, SLOT( onColorChanged() ) );
  connect( myOkBtn,     SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( myCancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
  setColor( QColor( 0, 0, 0 ) );

  topLayout->addWidget( TopGroup,     0, 0 );
  topLayout->addWidget( GroupButtons, 1, 0 );

  QAD_Tools::centerWidget( this, parent );
}
/*!
  Destructor
*/
SALOMEGUI_SetupCurveDlg::~SALOMEGUI_SetupCurveDlg()
{
}
/*!
  Sets line style and width
*/
void SALOMEGUI_SetupCurveDlg::setLine( const int line, const int width )
{
  myLineCombo->setCurrentItem( line );
  myLineSpin->setValue( width );
}
/*!
  Gets line style
*/
int SALOMEGUI_SetupCurveDlg::getLine() const
{
  return myLineCombo->currentItem();
}
/*!
  Gets line width
*/
int SALOMEGUI_SetupCurveDlg::getLineWidth() const
{
  return myLineSpin->value();
}
/*!
  Sets marker style
*/
void SALOMEGUI_SetupCurveDlg::setMarker( const int marker )
{
  myMarkerCombo->setCurrentItem( marker );
}
/*!
  Gets marker style
*/
int SALOMEGUI_SetupCurveDlg::getMarker() const 
{
  return myMarkerCombo->currentItem();
}
/*!
  Sets color
*/
void SALOMEGUI_SetupCurveDlg::setColor( const QColor& color )
{
  QPalette pal = myColorBtn->palette();
  QColorGroup ca = pal.active();
  ca.setColor( QColorGroup::Button, color );
  QColorGroup ci = pal.inactive();
  ci.setColor( QColorGroup::Button, color );
  pal.setActive( ca );
  pal.setInactive( ci );
  myColorBtn->setPalette( pal );
}
/*!
  Gets color
*/
QColor SALOMEGUI_SetupCurveDlg::getColor() const 
{
  return myColorBtn->palette().active().button();
}
/*!
  <Color> button slot, invokes color selection dialog box
*/
void SALOMEGUI_SetupCurveDlg::onColorChanged()
{
  QColor color = QColorDialog::getColor( getColor() );
  if ( color.isValid() ) {
    setColor( color );
  }
}





