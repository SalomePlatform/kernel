using namespace std;
//  File      : ToolsGUI_CatalogGeneratorDlg.cxx
//  Created   : Thu Apr 25 18:52:40 2002
//  Author    : Nicolas REJNERI

//  Project   : SALOME
//  Module    : TOOLSGUI
//  Copyright : Open CASCADE 2002
//  $Header$

#include "ToolsGUI_CatalogGeneratorDlg.h"

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_FileDlg.h"
#include "QAD_MessageBox.h"
#include <stdlib.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qvalidator.h>

#include <OSD_Process.hxx>
#include <OSD_Path.hxx>
#include <TCollection_AsciiString.hxx>
#include <Standard_CString.hxx>

#define SPACING_SIZE             6
#define MARGIN_SIZE             11
#define MIN_EDIT_SIZE          250

//=================================================================================
// class    : ToolsGUI_CatalogGeneratorDlg()
// purpose  : Constructor
//=================================================================================
ToolsGUI_CatalogGeneratorDlg::ToolsGUI_CatalogGeneratorDlg( QWidget* parent, const char* name )
    : QDialog( parent, name, TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  if ( !name )
    setName( "ToolsGUI_CatalogGeneratorDlg" );
  resize( 322, 120 ); 
  setCaption( tr( "TOOLS_CATALOG_GENERATOR" ) );
  setSizeGripEnabled( TRUE );

  QGridLayout* aTopLayout = new QGridLayout(this);
  aTopLayout->setMargin(MARGIN_SIZE);
  aTopLayout->setSpacing(SPACING_SIZE);

  QGroupBox* filesGrp = new QGroupBox( tr( "TOOLS_FILES") , this, "filesGrp" );
  filesGrp->setColumnLayout( 0, Qt::Vertical );
  filesGrp->layout()->setSpacing( 0 );
  filesGrp->layout()->setMargin( 0 );
  QGridLayout* filesGrpLayout = new QGridLayout( filesGrp->layout() );
  filesGrpLayout->setAlignment( Qt::AlignTop );
  filesGrpLayout->setSpacing( SPACING_SIZE );
  filesGrpLayout->setMargin( MARGIN_SIZE  );

  myIdlEdit = new QLineEdit( filesGrp, "myIdlEdit" );
  myIdlEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myIdlEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );
  myXmlEdit = new QLineEdit( filesGrp, "myXmlEdit" );
  myXmlEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myXmlEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );

  myBrowseIdlBtn = new QPushButton( tr( "TOOLS_BUT_BROWSE" ), filesGrp, "myBrowseIdlBtn" );
  myBrowseXmlBtn = new QPushButton( tr( "TOOLS_BUT_BROWSE" ), filesGrp, "myBrowseXmlBtn" );
//  QFontMetrics fm(myBrowseIdlBtn->font());
//  myBrowseIdlBtn->setFixedWidth(fm.width(myBrowseIdlBtn->text()) + 10);
//  myBrowseXmlBtn->setFixedWidth(fm.width(myBrowseXmlBtn->text()) + 10);

  filesGrpLayout->addWidget( new QLabel( tr( "TOOLS_IDL_FILE" ), filesGrp ), 0, 0);
  filesGrpLayout->addWidget( myIdlEdit, 0, 1 );
  filesGrpLayout->addWidget( myBrowseIdlBtn, 0, 2 );
  filesGrpLayout->addWidget( new QLabel( tr( "TOOLS_XML_FILE" ), filesGrp ), 1, 0);
  filesGrpLayout->addWidget( myXmlEdit, 1, 1 );
  filesGrpLayout->addWidget( myBrowseXmlBtn, 1, 2 );

  QGroupBox* supplGrp = new QGroupBox(tr( "TOOLS_SUPPLEMENT" )  , this, "SupplGrp" );
  supplGrp->setColumnLayout( 0, Qt::Vertical );
  supplGrp->layout()->setSpacing( 0 );
  supplGrp->layout()->setMargin( 0 );
  QGridLayout* supplGrpLayout = new QGridLayout( supplGrp->layout() );
  supplGrpLayout->setAlignment( Qt::AlignTop );
  supplGrpLayout->setSpacing( SPACING_SIZE );
  supplGrpLayout->setMargin( MARGIN_SIZE  );

  myAuthorEdit = new QLineEdit( supplGrp , "myAuthorEdit" );
  myAuthorEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myAuthorEdit->setMinimumSize( MIN_EDIT_SIZE*0.3, 0 );

  OSD_Process aProcess;
  myAuthorEdit->setText(aProcess.UserName().ToCString());

  myVersionEdit = new QLineEdit(supplGrp , "myVersion" );
  myVersionEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myVersionEdit->setMinimumSize( MIN_EDIT_SIZE*0.3, 0 );
  QStringList aList = QStringList::split(QRegExp("\\s+"),tr( "INF_VERSION" ));
  myVersionEdit->setText(aList.last());
  QDoubleValidator *validator = new QDoubleValidator(myVersionEdit);
  myVersionEdit->setValidator(validator);

  myPngEdit = new QLineEdit(supplGrp , "myCompIcon" );
  myPngEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myPngEdit->setMinimumSize( MIN_EDIT_SIZE, 0 );

  myBrowsePngBtn = new QPushButton( tr( "TOOLS_BUT_BROWSE" ), supplGrp, "myBrowsePngBtn" );

  myCompName = new QLineEdit(supplGrp , "myCompName");
  myCompName->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myCompName->setMinimumSize( MIN_EDIT_SIZE*0.3, 0 );

  myCompType = new QLineEdit(supplGrp , "myCompType");
  myCompType->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myCompType->setMinimumSize( MIN_EDIT_SIZE*0.3, 0 );
  myCompType->setText("OTHER");

  myCompMultiStd = new QLineEdit(supplGrp , "myCompMultiStd");
  myCompMultiStd->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
  myCompMultiStd->setMinimumSize( MIN_EDIT_SIZE*0.3, 0 );
  myCompMultiStd->setText("1");
  QIntValidator *ivalidator = new QIntValidator(myVersionEdit);
  myCompMultiStd->setValidator(ivalidator);
  
  supplGrpLayout->addWidget( new QLabel( tr( "TOOLS_AUTHOR" ), supplGrp ), 0, 0);
  supplGrpLayout->addWidget( myAuthorEdit, 0, 1 );
  supplGrpLayout->addWidget( new QLabel( tr( "TOOLS_COMP_NAME" ), supplGrp ), 0, 2);
  supplGrpLayout->addWidget(myCompName,0,3);
  supplGrpLayout->addWidget( new QLabel( tr( "TOOLS_COMP_TYPE" ), supplGrp ), 0, 4);
  supplGrpLayout->addWidget(myCompType,0,5);
  supplGrpLayout->addWidget( new QLabel( tr( "TOOLS_VERSION" ), supplGrp ), 1, 0);
  supplGrpLayout->addWidget( myVersionEdit, 1, 1);
  supplGrpLayout->addWidget( new QLabel( tr( "TOOLS_COMP_MULTISTD" ), supplGrp ), 1, 2);
  supplGrpLayout->addWidget(myCompMultiStd,1,3);
  supplGrpLayout->addWidget( new QLabel( tr( "TOOLS_PNG_FILE" ), supplGrp ), 2, 0);
  supplGrpLayout->addMultiCellWidget( myPngEdit, 2,2,1,4 );
  supplGrpLayout->addWidget( myBrowsePngBtn, 2, 5 );
  

  QHBoxLayout* aBtnLayout = new QHBoxLayout;
  aBtnLayout->setSpacing( SPACING_SIZE );
  aBtnLayout->setMargin( 0 );

  myApplyBtn = new QPushButton( tr( "TOOLS_BUT_APPLY"  ), this, "myApplyBtn" );
  myApplyBtn->setAutoDefault( true );
  myApplyBtn->setDefault( true );
  myCloseBtn = new QPushButton( tr( "TOOLS_BUT_CLOSE" ), this, "myCloseBtn" );
  myCloseBtn->setAutoDefault( true );
  
  aBtnLayout->addWidget( myApplyBtn );
  aBtnLayout->addItem( new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
  aBtnLayout->addWidget( myCloseBtn );

  aTopLayout->addWidget( filesGrp, 0, 0 );
  aTopLayout->addWidget( supplGrp, 1, 0 );
  aTopLayout->addLayout( aBtnLayout, 2, 0 ); 

  /* signals and slots connections */
  connect( myApplyBtn,     SIGNAL( clicked() ), this, SLOT( onApply() ) );
  connect( myCloseBtn,     SIGNAL( clicked() ), this, SLOT( reject() ) );
  connect( myBrowseIdlBtn, SIGNAL( clicked() ), this, SLOT( onBrowseBtnClicked() ) );
  connect( myBrowseXmlBtn, SIGNAL( clicked() ), this, SLOT( onBrowseBtnClicked() ) );
  connect( myBrowsePngBtn, SIGNAL( clicked() ), this, SLOT( onBrowseBtnClicked() ) );
  connect( myIdlEdit,      SIGNAL( textChanged( const QString& ) ), this, SLOT( updateButtonState() ) );
  connect( myXmlEdit,      SIGNAL( textChanged( const QString& ) ), this, SLOT( updateButtonState() ) );

  updateButtonState();
}

//=================================================================================
// function : ~ToolsGUI_CatalogGeneratorDlg()
// purpose  : destructor
//=================================================================================
ToolsGUI_CatalogGeneratorDlg::~ToolsGUI_CatalogGeneratorDlg()
{  
}

//=================================================================================
// function : getIdlFile()
// purpose  : gets IDL file name entered
//=================================================================================
QString ToolsGUI_CatalogGeneratorDlg::getIdlFile()
{
  return myIdlEdit->text().stripWhiteSpace();
}

//=================================================================================
// function : getXmlFile()
// purpose  : gets XML file name entered
//=================================================================================
QString ToolsGUI_CatalogGeneratorDlg::getXmlFile()
{
  return myXmlEdit->text().stripWhiteSpace();
}

//=================================================================================
// function : getPngFile()
// purpose  : gets PNG file name entered
//=================================================================================
QString ToolsGUI_CatalogGeneratorDlg::getPngFile()
{
  return myPngEdit->text().stripWhiteSpace();
}

//=================================================================================
// function : getAuthor()
// purpose  : gets author 
//=================================================================================
QString ToolsGUI_CatalogGeneratorDlg::getAuthor()
{
  return myAuthorEdit->text().stripWhiteSpace();
}

//=================================================================================
// function : getVersion()
// purpose  : gets version number 
//=================================================================================
QString ToolsGUI_CatalogGeneratorDlg::getVersion()
{
  return myVersionEdit->text().stripWhiteSpace();
}

//=================================================================================
// function : getCompName()
// purpose  : gets name of the component
//=================================================================================
QString ToolsGUI_CatalogGeneratorDlg::getCompName()
{
  return myCompName->text().stripWhiteSpace();
}

//=================================================================================
// function : getCompType()
// purpose  : gets type of the component
//=================================================================================
QString ToolsGUI_CatalogGeneratorDlg::getCompMultiStd()
{
  return myCompMultiStd->text().stripWhiteSpace();
}
//=================================================================================
// function : getComptype()
// purpose  : gets type of the component
//=================================================================================
QString ToolsGUI_CatalogGeneratorDlg::getCompType()
{
  return myCompType->text().stripWhiteSpace();
}

//=================================================================================
// function : onBrowseBtnClicked()
// purpose  : <...> (Browse) buttons slot
//=================================================================================
void ToolsGUI_CatalogGeneratorDlg::onBrowseBtnClicked()
{
  QPushButton* send = (QPushButton*)sender();
 
  if ( send == myBrowseIdlBtn ) {
    QString file = myIdlEdit->text().stripWhiteSpace();
    file = QAD_FileDlg::getFileName(QAD_Application::getDesktop(), 
				    file,
				    tr("TOOLS_MEN_IMPORT_IDL"),
				    tr("TOOLS_MEN_IMPORT"),
				    true);
    if ( !file.isEmpty() ) {
      myIdlEdit->setText(file);
    }
  } 
  else if ( send == myBrowseXmlBtn ) {
    QString file = QAD_FileDlg::getFileName(QAD_Application::getDesktop(), 
					    myXmlEdit->text().stripWhiteSpace(),
					    tr("TOOLS_MEN_EXPORT_XML"),
					    tr("TOOLS_MEN_EXPORT"),
					    false);
    if ( !file.isEmpty() ) {
      myXmlEdit->setText(file);
    }
  } else if ( send == myBrowsePngBtn ) {
    QString file = QAD_FileDlg::getFileName(QAD_Application::getDesktop(), 
					    myXmlEdit->text().stripWhiteSpace(),
					    tr("TOOLS_MEN_IMPORT_PNG"),
					    tr("TOOLS_MEN_IMPORT"),
					    true);
    if ( !file.isEmpty() ) {
      myPngEdit->setText(file);
    
    }
  }
  updateButtonState();
}

//=================================================================================
// function : updateButtonState()
// purpose  : Updates <OK> button's state
//=================================================================================
void ToolsGUI_CatalogGeneratorDlg::updateButtonState()
{
  myApplyBtn->setEnabled( !myIdlEdit->text().stripWhiteSpace().isEmpty() && 
			  !myXmlEdit->text().stripWhiteSpace().isEmpty() );
}

//=================================================================================
// function : onApply()
// purpose  : <Apply> button slot, performs IDL->XML conversion
//=================================================================================
void ToolsGUI_CatalogGeneratorDlg::onApply()
{
  QString XmlFile = getXmlFile();
  QString IdlFile = getIdlFile();
  QString Author  = getAuthor();
  QString Version = getVersion();
  QString PngFile = getPngFile();
  QString CompName = getCompName(); //gets component name 
  QString CompType = getCompType(); //gets component type
  QString CompMultiStd = getCompMultiStd();

  if ( !XmlFile.isEmpty() && !IdlFile.isEmpty() ) {
    if ( !QFile::exists( IdlFile ) ) {
      QAD_MessageBox::error1( this, 
			      tr("TOOLS_ERR_ERROR"), 
			      tr("TOOLS_ERR_FILE_NOT_EXIST").arg(IdlFile), 
			      tr ("TOOLS_BUT_OK") );
    }
    else {
      QString command = "";
      if ( getenv("SALOME_ROOT_DIR")  )
	command = QString( getenv( "SALOME_ROOT_DIR" ) ) + "/bin/runIDLparser -Wbcatalog=" + XmlFile;
      else if ( getenv("KERNEL_ROOT_DIR")  )
	command = QString( getenv( "KERNEL_ROOT_DIR" ) ) + "/bin/runIDLparser -Wbcatalog=" + XmlFile;
      else {
	QAD_MessageBox::error1( this, 
				tr("TOOLS_ERR_ERROR"), 
				tr("SALOME_ROOT_DIR or KERNEL_ROOT_DIR variables are not defined"), 
				tr("TOOLS_BUT_OK") );
      }
      if (!Author.isEmpty()) command += ",author=" + Author; 
      if (!Version.isEmpty()) command += ",version=" + Version;
      if (!PngFile.isEmpty()) {
	OSD_Path aPath((Standard_CString)PngFile.latin1()); 
	TCollection_AsciiString aFile = aPath.Name() + aPath.Extension();
	command += QString(",icon=") + QString(aFile.ToCString());
      }
      if (!CompName.isEmpty()) command += ",name=" + CompName;
      if (!CompType.isEmpty()) command += ",type=" + CompType;
      if (!CompMultiStd.isEmpty()) command += ",multistudy=" + CompMultiStd;
      command += " " + IdlFile;
      MESSAGE( "shell command is : " << command );
      int res;
      res = system( ( char* )( command.latin1() ) );
      if ( res == -1 ) {
	MESSAGE( "work failed (system command result = " << res );
      } else if (res == 217) {
	MESSAGE( "shell exec failed (system command result = " << res );
      }
    }
  }
}
