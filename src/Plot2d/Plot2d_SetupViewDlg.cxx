//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_SetupViewDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "Plot2d_SetupViewDlg.h"
#include "QAD_Tools.h"

#include <qcheckbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcolordialog.h>
using namespace std;

#define MARGIN_SIZE          11
#define SPACING_SIZE         6
#define MIN_EDIT_WIDTH       200
#define MIN_COMBO_WIDTH      100
#define MIN_SPIN_WIDTH       70

/*!
  Constructor
*/
Plot2d_SetupViewDlg::Plot2d_SetupViewDlg( QWidget* parent, bool showDefCheck )
    : QDialog( parent, "Plot2d_SetupViewDlg", true, 
	       WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  setCaption( tr("TLT_SETUP_PLOT2D_VIEW") );
  setSizeGripEnabled( TRUE );
  QGridLayout* topLayout = new QGridLayout( this ); 
  topLayout->setSpacing( SPACING_SIZE );
  topLayout->setMargin( MARGIN_SIZE );
  
  // main title
  myTitleCheck = new QCheckBox( tr( "PLOT2D_ENABLE_MAIN_TITLE" ), this );
  myTitleEdit  = new QLineEdit( this );
  myTitleEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myTitleEdit->setMinimumWidth( MIN_EDIT_WIDTH );
  // horizontal axis title
  myTitleXCheck = new QCheckBox( tr( "PLOT2D_ENABLE_HOR_TITLE" ), this );
  myTitleXEdit  = new QLineEdit( this );
  myTitleXEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myTitleXEdit->setMinimumWidth( MIN_EDIT_WIDTH );
  // vertical axis title
  myTitleYCheck = new QCheckBox( tr( "PLOT2D_ENABLE_VER_TITLE" ), this );
  myTitleYEdit  = new QLineEdit( this );
  myTitleYEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myTitleYEdit->setMinimumWidth( MIN_EDIT_WIDTH );
  // curve type : points, lines, spline
  QLabel* aCurveLab = new QLabel( tr( "PLOT2D_CURVE_TYPE_LBL" ), this );
  myCurveCombo      = new QComboBox( false, this );
  myCurveCombo->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myCurveCombo->setMinimumWidth( MIN_COMBO_WIDTH );
  myCurveCombo->insertItem( tr( "PLOT2D_CURVE_TYPE_POINTS" ) );
  myCurveCombo->insertItem( tr( "PLOT2D_CURVE_TYPE_LINES" ) );
  myCurveCombo->insertItem( tr( "PLOT2D_CURVE_TYPE_SPLINE" ) );
  // legend
  myLegendCheck = new QCheckBox( tr( "PLOT2D_ENABLE_LEGEND" ), this );
  myLegendCombo = new QComboBox( false, this );
  myLegendCombo->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myLegendCombo->setMinimumWidth( MIN_COMBO_WIDTH );
  myLegendCombo->insertItem( tr( "PLOT2D_LEGEND_POSITION_LEFT" ) );
  myLegendCombo->insertItem( tr( "PLOT2D_LEGEND_POSITION_RIGHT" ) );
  myLegendCombo->insertItem( tr( "PLOT2D_LEGEND_POSITION_TOP" ) );
  myLegendCombo->insertItem( tr( "PLOT2D_LEGEND_POSITION_BOTTOM" ) );
  // marker size
  QLabel* aMarkerLab  = new QLabel( tr( "PLOT2D_MARKER_SIZE_LBL" ), this );
  myMarkerSpin = new QSpinBox( 0, 100, 1, this );
  myMarkerSpin->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myMarkerSpin->setMinimumWidth( MIN_SPIN_WIDTH );
  // background color
  QLabel* aBGLab  = new QLabel( tr( "PLOT2D_BACKGROUND_COLOR_LBL" ), this );
  myBackgroundBtn = new QToolButton( this );
  // scale mode
  QGroupBox* aScaleGrp = new QGroupBox( tr( "PLOT2D_SCALE_TLT" ), this );
  aScaleGrp->setColumnLayout(0, Qt::Vertical );
  aScaleGrp->layout()->setSpacing( 0 );  aScaleGrp->layout()->setMargin( 0 );
  QGridLayout* aScaleLayout = new QGridLayout( aScaleGrp->layout() );
  aScaleLayout->setMargin( MARGIN_SIZE ); aScaleLayout->setSpacing( SPACING_SIZE );

  QLabel* xScaleLab = new QLabel( tr( "PLOT2D_SCALE_MODE_HOR" ), aScaleGrp );
  myXModeCombo = new QComboBox( false, aScaleGrp );
  myXModeCombo->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myXModeCombo->setMinimumWidth( MIN_COMBO_WIDTH );
  myXModeCombo->insertItem( tr( "PLOT2D_SCALE_MODE_LINEAR" ) );
  myXModeCombo->insertItem( tr( "PLOT2D_SCALE_MODE_LOGARITHMIC" ) );
  QLabel* yScaleLab = new QLabel( tr( "PLOT2D_SCALE_MODE_VER" ), aScaleGrp );
  myYModeCombo = new QComboBox( false, aScaleGrp );
  myYModeCombo->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myYModeCombo->setMinimumWidth( MIN_COMBO_WIDTH );
  myYModeCombo->insertItem( tr( "PLOT2D_SCALE_MODE_LINEAR" ) );
  myYModeCombo->insertItem( tr( "PLOT2D_SCALE_MODE_LOGARITHMIC" ) );

  aScaleLayout->addWidget( xScaleLab,    0, 0 );
  aScaleLayout->addWidget( myXModeCombo, 0, 1 );
  aScaleLayout->addWidget( yScaleLab,    0, 2 );
  aScaleLayout->addWidget( myYModeCombo, 0, 3 );

  // grid
  QGroupBox* aGridGrp = new QGroupBox( tr( "PLOT2D_GRID_TLT" ), this );
  aGridGrp->setColumnLayout(0, Qt::Vertical );
  aGridGrp->layout()->setSpacing( 0 );  aGridGrp->layout()->setMargin( 0 );
  QGridLayout* aGridLayout = new QGridLayout( aGridGrp->layout() );
  aGridLayout->setMargin( MARGIN_SIZE ); aGridLayout->setSpacing( SPACING_SIZE );

  myXGridCheck      = new QCheckBox( tr( "PLOT2D_GRID_ENABLE_HOR_MAJOR" ), aGridGrp );
  myXGridSpin       = new QSpinBox( 1, 100, 1, aGridGrp );
  myXGridSpin->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myXGridSpin->setMinimumWidth( MIN_SPIN_WIDTH );
  myYGridCheck      = new QCheckBox( tr( "PLOT2D_GRID_ENABLE_VER_MAJOR" ), aGridGrp );
  myYGridSpin       = new QSpinBox( 1, 100, 1, aGridGrp );
  myYGridSpin->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myYGridSpin->setMinimumWidth( MIN_SPIN_WIDTH );
  myXMinGridCheck      = new QCheckBox( tr( "PLOT2D_GRID_ENABLE_HOR_MINOR" ), aGridGrp );
  myXMinGridSpin       = new QSpinBox( 1, 100, 1, aGridGrp );
  myXMinGridSpin->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myXMinGridSpin->setMinimumWidth( MIN_SPIN_WIDTH );
  myYMinGridCheck      = new QCheckBox( tr( "PLOT2D_GRID_ENABLE_VER_MINOR" ), aGridGrp );
  myYMinGridSpin       = new QSpinBox( 1, 100, 1, aGridGrp );
  myYMinGridSpin->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myYMinGridSpin->setMinimumWidth( MIN_SPIN_WIDTH );

  aGridLayout->addWidget( myXGridCheck,    0, 0 );
  aGridLayout->addWidget( myXGridSpin,     0, 1 );
  aGridLayout->addWidget( myYGridCheck,    0, 2 );
  aGridLayout->addWidget( myYGridSpin,     0, 3 );
  aGridLayout->addWidget( myXMinGridCheck, 1, 0 );
  aGridLayout->addWidget( myXMinGridSpin,  1, 1 );
  aGridLayout->addWidget( myYMinGridCheck, 1, 2 );
  aGridLayout->addWidget( myYMinGridSpin,  1, 3 );

  /* "Set as default" check box */
  myDefCheck = new QCheckBox( tr( "PLOT2D_SET_AS_DEFAULT_CHECK" ), this );

  /* OK/Cancel buttons */
  myOkBtn = new QPushButton( tr( "BUT_OK" ), this );
  myOkBtn->setAutoDefault( TRUE );
  myOkBtn->setDefault( TRUE );
  myCancelBtn = new QPushButton( tr( "BUT_CANCEL" ), this );
  myCancelBtn->setAutoDefault( TRUE );
  QHBoxLayout* btnLayout = new QHBoxLayout;
  btnLayout->addWidget( myOkBtn );
  btnLayout->addStretch();
  btnLayout->addWidget( myCancelBtn );
  
  // layout widgets
  topLayout->addWidget( myTitleCheck,          0,    0    );
  topLayout->addMultiCellWidget( myTitleEdit,  0, 0, 1, 3 );
  topLayout->addWidget( myTitleXCheck,         1,    0    );
  topLayout->addMultiCellWidget( myTitleXEdit, 1, 1, 1, 3 );
  topLayout->addWidget( myTitleYCheck,         2,    0    );
  topLayout->addMultiCellWidget( myTitleYEdit, 2, 2, 1, 3 );
  topLayout->addWidget( aCurveLab,             3,    0    );
  topLayout->addWidget( myCurveCombo,          3,    1    );
  topLayout->addWidget( myLegendCheck,         3,    2    );
  topLayout->addWidget( myLegendCombo,         3,    3    );
  topLayout->addWidget( aMarkerLab,            4,    0    );
  topLayout->addWidget( myMarkerSpin,          4,    1    );
  QHBoxLayout* bgLayout = new QHBoxLayout;
  bgLayout->addWidget( myBackgroundBtn ); bgLayout->addStretch();
  topLayout->addWidget( aBGLab,                4,    2    );
  topLayout->addLayout( bgLayout,              4,    3    );
  topLayout->addMultiCellWidget( aScaleGrp,    5, 5, 0, 3 );
  topLayout->addMultiCellWidget( aGridGrp,     6, 6, 0, 3 );
  topLayout->addMultiCellWidget( myDefCheck,   7, 7, 0, 3 );
  topLayout->setRowStretch( 8, 5 );
  topLayout->addMultiCellLayout( btnLayout,    9, 9, 0, 3 );
  
  if ( !showDefCheck )
    myDefCheck->hide();

  connect( myTitleCheck,    SIGNAL( clicked() ), this, SLOT( onMainTitleChecked() ) );
  connect( myTitleXCheck,   SIGNAL( clicked() ), this, SLOT( onXTitleChecked() ) );
  connect( myTitleYCheck,   SIGNAL( clicked() ), this, SLOT( onYTitleChecked() ) );
  connect( myBackgroundBtn, SIGNAL( clicked() ), this, SLOT( onBackgroundClicked() ) );
  connect( myLegendCheck,   SIGNAL( clicked() ), this, SLOT( onLegendChecked() ) );
  connect( myXGridCheck,    SIGNAL( clicked() ), this, SLOT( onXGridMajorChecked() ) );
  connect( myYGridCheck,    SIGNAL( clicked() ), this, SLOT( onYGridMajorChecked() ) );
  connect( myXMinGridCheck, SIGNAL( clicked() ), this, SLOT( onXGridMinorChecked() ) );
  connect( myYMinGridCheck, SIGNAL( clicked() ), this, SLOT( onYGridMinorChecked() ) );

  connect( myOkBtn,         SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( myCancelBtn,     SIGNAL( clicked() ), this, SLOT( reject() ) );

  /* Center widget inside it's parent widget */
  QAD_Tools::centerWidget( this, parentWidget() );

  // init fields
  setBackgroundColor( Qt::gray );
  onMainTitleChecked();
  onXTitleChecked();
  onYTitleChecked();
  onLegendChecked();
  onXGridMajorChecked();
  onYGridMajorChecked();
  onXGridMinorChecked();
  onYGridMinorChecked();
}

/*!
  Destructor
*/
Plot2d_SetupViewDlg::~Plot2d_SetupViewDlg()
{
}
/*!
  Sets main title attributes
*/
void Plot2d_SetupViewDlg::setMainTitle( bool enable, const QString& title )
{
  myTitleCheck->setChecked( enable );
  if ( !title.isNull() )
    myTitleEdit->setText( title );
  onMainTitleChecked();
}
/*!
  Returns TRUE if main title is enabled
*/
bool Plot2d_SetupViewDlg::isMainTitleEnabled()
{
  return myTitleCheck->isChecked();
}
/*!
  Gets main title
*/
QString Plot2d_SetupViewDlg::getMainTitle()
{
  return myTitleEdit->text();
}
/*!
  Sets horizontal axis title attributes
*/
void Plot2d_SetupViewDlg::setXTitle( bool enable, const QString& title )
{
  myTitleXCheck->setChecked( enable );
  if ( !title.isNull() )
    myTitleXEdit->setText( title );
  onXTitleChecked();
}
/*!
  Returns TRUE if horizontal axis title is enabled
*/
bool Plot2d_SetupViewDlg::isXTitleEnabled()
{
  return myTitleXCheck->isChecked();
}
/*!
  Gets horizontal axis title
*/
QString Plot2d_SetupViewDlg::getXTitle()
{
  return myTitleXEdit->text();
}
/*!
  Sets horizontal axis title attributes
*/
void Plot2d_SetupViewDlg::setYTitle( bool enable, const QString& title )
{
  myTitleYCheck->setChecked( enable );
  if ( !title.isNull() )
    myTitleYEdit->setText( title );
  onYTitleChecked();
}
/*!
  Returns TRUE if vertical axis title is enabled
*/
bool Plot2d_SetupViewDlg::isYTitleEnabled()
{
  return myTitleYCheck->isChecked();
}
/*!
  Gets vertical axis title
*/
QString Plot2d_SetupViewDlg::getYTitle()
{
  return myTitleYEdit->text();
}
/*!
  Sets curve type : 0 - points, 1 - lines, 2 - splines
*/
void Plot2d_SetupViewDlg::setCurveType( const int type )
{
  myCurveCombo->setCurrentItem( type );
}
/*!
  Gets curve type : 0 - points, 1 - lines, 2 - splines
*/
int Plot2d_SetupViewDlg::getCurveType()
{
  return myCurveCombo->currentItem();
}
/*!
  Sets legend attributes : pos = 0 - left, 1 - right, 2 - top, 3 - bottom
*/
void Plot2d_SetupViewDlg::setLegend( bool enable, int pos )
{
  myLegendCheck->setChecked( enable );
  myLegendCombo->setCurrentItem( pos );
  onLegendChecked();
}
/*!
  Returns TRUE if legend is enabled
*/
bool Plot2d_SetupViewDlg::isLegendEnabled()
{
  return myLegendCheck->isChecked();
}
/*!
  Returns legend position
*/
int Plot2d_SetupViewDlg::getLegendPos()
{
  return myLegendCombo->currentItem();
}
/*!
  Sets marker size
*/
void Plot2d_SetupViewDlg::setMarkerSize( const int size )
{
  myMarkerSpin->setValue( size );
}
/*!
  Gets marker size
*/
int Plot2d_SetupViewDlg::getMarkerSize()
{
  return myMarkerSpin->value();
}
/*!
  Sets background color
*/
void Plot2d_SetupViewDlg::setBackgroundColor( const QColor& color )
{
  QPalette pal = myBackgroundBtn->palette();
  QColorGroup ca = pal.active();
  ca.setColor( QColorGroup::Button, color );
  QColorGroup ci = pal.inactive();
  ci.setColor( QColorGroup::Button, color );
  pal.setActive( ca );
  pal.setInactive( ci );
  myBackgroundBtn->setPalette( pal );
}
/*!
  Gets background color
*/
QColor Plot2d_SetupViewDlg::getBackgroundColor()
{
  return myBackgroundBtn->palette().active().button();
}
/*!
  Sets major grid parameters
*/
void Plot2d_SetupViewDlg::setMajorGrid( bool enableX, const int divX, bool enableY, const int divY )
{
  myXGridCheck->setChecked( enableX );
  myXGridSpin->setValue( divX );
  myYGridCheck->setChecked( enableY );
  myYGridSpin->setValue( divY );
  onXGridMajorChecked();
  onYGridMajorChecked();
}
/*!
  Gets major grid parameters
*/
void Plot2d_SetupViewDlg::getMajorGrid( bool& enableX, int& divX, bool& enableY, int& divY )
{
  enableX = myXGridCheck->isChecked();
  divX    = myXGridSpin->value();
  enableY = myYGridCheck->isChecked();
  divY    = myYGridSpin->value();
}
/*!
  Sets minor grid parameters
*/
void Plot2d_SetupViewDlg::setMinorGrid( bool enableX, const int divX, bool enableY, const int divY )
{
  myXMinGridCheck->setChecked( enableX );
  myXMinGridSpin->setValue( divX );
  myYMinGridCheck->setChecked( enableY );
  myYMinGridSpin->setValue( divY );
  onXGridMinorChecked();
  onYGridMinorChecked();
}
/*!
  Gets minor grid parameters
*/
void Plot2d_SetupViewDlg::getMinorGrid( bool& enableX, int& divX, bool& enableY, int& divY )
{
  enableX = myXMinGridCheck->isChecked();
  divX    = myXMinGridSpin->value();
  enableY = myYMinGridCheck->isChecked();
  divY    = myYMinGridSpin->value();
}
/*!
  Sets scale mode for hor. and ver. axes : 0 - linear, 1 - logarithmic
*/
void Plot2d_SetupViewDlg::setScaleMode( const int xMode, const int yMode )
{
  myXModeCombo->setCurrentItem( xMode );
  myYModeCombo->setCurrentItem( yMode );
}
/*!
  Gets scale mode for hor. axis : 0 - linear, 1 - logarithmic
*/
int  Plot2d_SetupViewDlg::getXScaleMode()
{
  return myXModeCombo->currentItem();
}
/*!
  Gets scale mode for hor. axis : 0 - linear, 1 - logarithmic
*/
int  Plot2d_SetupViewDlg::getYScaleMode()
{
  return myYModeCombo->currentItem();
}
/*!
  Slot, called when user clicks "Show main title" check box
*/
void Plot2d_SetupViewDlg::onMainTitleChecked()
{
  myTitleEdit->setEnabled( myTitleCheck->isChecked() );
}
/*!
  Slot, called when user clicks "Show horizontal axis title" check box
*/
void Plot2d_SetupViewDlg::onXTitleChecked()
{
  myTitleXEdit->setEnabled( myTitleXCheck->isChecked() );
}
/*!
  Slot, called when user clicks "Show vertical axis title" check box
*/
void Plot2d_SetupViewDlg::onYTitleChecked()
{
  myTitleYEdit->setEnabled( myTitleYCheck->isChecked() );
}
/*!
  Slot, called when user clicks "Change bacground color" button
*/
void Plot2d_SetupViewDlg::onBackgroundClicked()
{
  QColor color = QColorDialog::getColor( getBackgroundColor() );
  if ( color.isValid() ) {
    setBackgroundColor( color );
  }
}
/*!
  Slot, called when user clicks "Show Legend" check box
*/
void Plot2d_SetupViewDlg::onLegendChecked()
{
  myLegendCombo->setEnabled( myLegendCheck->isChecked() );
}
/*!
  Slot, called when user clicks "Enable hor. major grid" check box
*/
void Plot2d_SetupViewDlg::onXGridMajorChecked()
{
  myXMinGridCheck->setEnabled( myXGridCheck->isChecked() );
}
/*!
  Slot, called when user clicks  "Enable ver. major grid" check box
*/
void Plot2d_SetupViewDlg::onYGridMajorChecked()
{
  myYMinGridCheck->setEnabled( myYGridCheck->isChecked() );
}
/*!
  Slot, called when user clicks  "Enable hor. minor grid" check box
*/
void Plot2d_SetupViewDlg::onXGridMinorChecked()
{
}
/*!
  Slot, called when user clicks  "Enable ver. minor grid" check box
*/
void Plot2d_SetupViewDlg::onYGridMinorChecked()
{
}
/*!
  Retursns true if "Set as default" check box is on
*/
bool Plot2d_SetupViewDlg::isSetAsDefault()
{
  return myDefCheck->isChecked();
}
