//  File   : SALOMEGUI_ExternalBrowserDlg.cxx
//  Author : Nichael ZORIN
//  Module : SALOME
//  $Header: /dn05/salome/CVS/SALOME_ROOT/SALOME/src/SALOMEGUI/SALOMEGUI_ExternalBrowserDlg.cxx

using namespace std;
#include "SALOMEGUI_ExternalBrowserDlg.h"

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qlayout.h>

#include "QAD_FileDlg.h"
#include "QAD_Tools.h"

/* 
 *  Constructs a modal SALOMEGUI_ExternalBowserDlg which is a child of 'parent'
 *
 */

SALOMEGUI_ExternalBrowserDlg::SALOMEGUI_ExternalBrowserDlg( QWidget* parent )
  : QDialog( parent, "SALOMEGUI_ExternalBrowserDlg", true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  setCaption( tr( "MEN_EXTERNAL_BROWSER"  ) );
  setSizeGripEnabled( true );
 
  QGridLayout* TopLayout = new QGridLayout( this ); 
  TopLayout->setSpacing( 6 );
  TopLayout->setMargin( 11 );
  
  QGroupBox* TopGroup = new QGroupBox( this, "TopGroup" );
  TopGroup->setColumnLayout(0, Qt::Vertical );
  TopGroup->layout()->setSpacing( 0 );
  TopGroup->layout()->setMargin( 0 );
  QGridLayout* TopGroupLayout = new QGridLayout( TopGroup->layout() );
  TopGroupLayout->setAlignment( Qt::AlignTop );
  TopGroupLayout->setSpacing( 6 );
  TopGroupLayout->setMargin( 11 );
  
  QLabel* LabelApp = new QLabel( tr( "MEN_EXTERNAL_BROWSER_APP" ), TopGroup, "LabelApp" );
  TopGroupLayout->addWidget( LabelApp, 0, 0 );
  ApplicationLine = new QLineEdit( TopGroup );
  ApplicationLine->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  ApplicationLine->setMinimumSize( 250, 0 );
  LabelApp->setBuddy(ApplicationLine);
  TopGroupLayout->addWidget( ApplicationLine, 0, 1 );
  
  QLabel* LabelParams = new QLabel( tr( "MEN_EXTERNAL_BROWSER_PARAMS" ), TopGroup, "LabelParams" );
  TopGroupLayout->addWidget( LabelParams, 1, 0 );
  ParametersLine = new QLineEdit( TopGroup );
  ParametersLine->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  ParametersLine->setMinimumSize( 250, 0 );
  LabelParams->setBuddy(ParametersLine);
  TopGroupLayout->addWidget( ParametersLine, 1, 1 );
  
  browseButton = new QPushButton( tr( "MEN_EXTERNAL_BROWSER_BROWSE" ) , TopGroup, "browseButton" );
  TopGroupLayout->addWidget( browseButton, 0, 2 );
  
  QGroupBox* GroupButtons = new QGroupBox( this, "GroupButtons" );
  GroupButtons->setGeometry( QRect( 10, 10, 381, 48 ) ); 
  GroupButtons->setColumnLayout(0, Qt::Vertical );
  GroupButtons->layout()->setSpacing( 0 );
  GroupButtons->layout()->setMargin( 0 );
  QGridLayout* GroupButtonsLayout = new QGridLayout( GroupButtons->layout() );
  GroupButtonsLayout->setAlignment( Qt::AlignTop );
  GroupButtonsLayout->setSpacing( 6 );
  GroupButtonsLayout->setMargin( 11 );
  
  QPushButton* buttonOk = new QPushButton( tr( "BUT_OK" ), GroupButtons, "buttonOk" );
  buttonOk->setAutoDefault( TRUE );
  buttonOk->setDefault( TRUE );
  GroupButtonsLayout->addWidget( buttonOk, 0, 0 );
  GroupButtonsLayout->addItem( new QSpacerItem( 5, 5, QSizePolicy::Expanding, QSizePolicy::Minimum ), 0, 1 );
  QPushButton* buttonCancel = new QPushButton( tr( "BUT_CANCEL" ) , GroupButtons, "buttonCancel" );
  buttonCancel->setAutoDefault( TRUE );
  GroupButtonsLayout->addWidget( buttonCancel, 0, 2 );
  
  TopLayout->addWidget( TopGroup,     0, 0 );
  TopLayout->addWidget( GroupButtons, 1, 0 );
  
  // signals and slots connections
  connect( browseButton, SIGNAL( clicked() ), this, SLOT( onButtonClicked() ) );
  connect( buttonOk,     SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
  
}

void SALOMEGUI_ExternalBrowserDlg::onButtonClicked()
{
  QStringList flt;
  flt.append( "ALL Files(*)" );
  QString aDir = QAD_Tools::getDirFromPath ( ApplicationLine->text() );
  QFileInfo aFileInfo = QAD_FileDlg::getFileName (this, aDir, flt, tr( "MEN_EXTERNAL_BROWSER_CHOOSEAPP" ), true);
  if(aFileInfo.exists())
    ApplicationLine->setText(aFileInfo.filePath());
}

void SALOMEGUI_ExternalBrowserDlg::setSettings(const QString& AppString, const QString& ParamsString)
{
  ApplicationLine->setText(AppString);
  ParametersLine->setText(ParamsString);
}

QString SALOMEGUI_ExternalBrowserDlg::getApp()
{
  return ApplicationLine->text();
}

QString SALOMEGUI_ExternalBrowserDlg::getParams()
{
  return ParametersLine->text();
}

void SALOMEGUI_ExternalBrowserDlg::accept() {

  QFileInfo aFile = QFileInfo(ApplicationLine->text());
   
  if (aFile.isFile() && !aFile.isExecutable())
    QMessageBox::warning ( this, tr( "Error" ) , tr ( "MEN_EXTERNAL_BROWSER_NOTEXECUTABLE" ));
  else
    QDialog::accept();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
SALOMEGUI_ExternalBrowserDlg::~SALOMEGUI_ExternalBrowserDlg()
{
    // no need to delete child widgets, Qt does it all for us
}


