//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_FileDlg.cxx
//  Author : 
//  Module : SALOME
//  $Header$

#include <qapplication.h>
#include <qdir.h>
#include <qlabel.h>
#include <qobjectlist.h>
#include <qpalette.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include "QAD_Config.h"
#include "QAD_Desktop.h"   
#include "QAD_FileDlg.h"
#include "QAD_MessageBox.h"
#include "QAD_Tools.h"   
using namespace std;

#define MIN_COMBO_SIZE     100

QString QAD_FileDlg::myLastVisitedPath;

/*!
Constructor
*/
QAD_FileDlg::QAD_FileDlg( QWidget* parent, bool open, bool showQuickDir, bool modal ) :
QFileDialogP( parent, 0, modal ),
myValidator( 0 ),
myQuickCombo( 0 ),
myOpen( open )
{    
  if ( parent->icon() )
    setIcon( *parent->icon() );
  setSizeGripEnabled( true );

  if (showQuickDir) {
    // inserting quick dir combo box
    QLabel* lab  = new QLabel(tr("Quick path:"), this);
    myQuickCombo = new QComboBox(false, this);
    myQuickCombo->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    myQuickCombo->setMinimumSize(MIN_COMBO_SIZE, 0);
    
    // the following is a workaround for proper layouting of custom widgets ===========
    QPushButton* btn = new QPushButton(this);
    btn->setEnabled(false);
    QPalette pal = btn->palette();
    QColorGroup ca = pal.active();
    ca.setColor(QColorGroup::Light,    palette().active().background());
    ca.setColor(QColorGroup::Midlight, palette().active().background());
    ca.setColor(QColorGroup::Dark,     palette().active().background());
    ca.setColor(QColorGroup::Mid,      palette().active().background());
    ca.setColor(QColorGroup::Shadow,   palette().active().background());
    QColorGroup ci = pal.inactive();
    ci.setColor(QColorGroup::Light,    palette().inactive().background());
    ci.setColor(QColorGroup::Midlight, palette().inactive().background());
    ci.setColor(QColorGroup::Dark,     palette().inactive().background());
    ci.setColor(QColorGroup::Mid,      palette().inactive().background());
    ci.setColor(QColorGroup::Shadow,   palette().inactive().background());
    QColorGroup cd = pal.disabled();
    cd.setColor(QColorGroup::Light,    palette().disabled().background());
    cd.setColor(QColorGroup::Midlight, palette().disabled().background());
    cd.setColor(QColorGroup::Dark,     palette().disabled().background());
    cd.setColor(QColorGroup::Mid,      palette().disabled().background());
    cd.setColor(QColorGroup::Shadow,   palette().disabled().background());
    pal.setActive(ca); pal.setInactive(ci); pal.setDisabled(cd);
    btn->setPalette(pal);
    // ================================================================================

    connect(myQuickCombo, SIGNAL(activated(const QString&)), this, SLOT(quickDir(const QString&)));
    addWidgets(lab, myQuickCombo, btn);

    // getting dir list from settings
    QString dirs = QAD_CONFIG->getSetting("FileDlg:QuickDirList");
    QStringList dirList = QStringList::split(';', dirs, false);
    if (dirList.count() > 0) {
      for (unsigned i = 0; i < dirList.count(); i++)
	myQuickCombo->insertItem(dirList[i]);
    }
    else {
      myQuickCombo->insertItem(QDir::homeDirPath());
    }

    // the following is a workaround for proper layouting of custom widgets ===========
    QValueList<QPushButton*> buttonList;
    QValueList<QLabel*> labelList;
    const QObjectList *list = children();
    QObjectListIt it(*list);
    int maxButWidth = lab->sizeHint().width();
    int maxLabWidth = btn->sizeHint().width();
    
    for (; it.current() ; ++it) {
      if ( it.current()->isA( "QLabel" ) ) {
	int tempW = ((QLabel*)it.current())->minimumWidth();
	if ( maxLabWidth < tempW ) maxLabWidth = tempW;
	labelList.append( (QLabel*)it.current() );
      }
      else if( it.current()->isA("QPushButton") ) {
	int tempW = ((QPushButton*)it.current())->minimumWidth();
	if ( maxButWidth < tempW ) maxButWidth = tempW;
	buttonList.append( (QPushButton*)it.current() );
      }
    }
    if (maxButWidth > 0) {
      QValueList<QPushButton*>::Iterator bListIt;
      for ( bListIt = buttonList.begin(); bListIt != buttonList.end(); ++bListIt )
	(*bListIt)->setFixedWidth( maxButWidth );
    }
    if (maxLabWidth > 0) {
      QValueList<QLabel*>::Iterator lListIt;
      for ( lListIt = labelList.begin(); lListIt != labelList.end(); ++lListIt )
	(*lListIt)->setFixedWidth( maxLabWidth );
    }
    // ================================================================================
  }
  setMode( myOpen ? ExistingFile : AnyFile );     
  setCaption( myOpen ? tr( "INF_DESK_DOC_OPEN" ) : tr( "INF_DESK_DOC_SAVE" ) );
  if (myLastVisitedPath.isNull() || myLastVisitedPath.isEmpty()) {
    // If no last visited path exists -> switch to the first preferred path
    processPath(myQuickCombo->text(0));
  } 
  else if ( !processPath(myLastVisitedPath) ) {
    // If last visited path doesn't exist -> switch to the first preferred path
    processPath(myQuickCombo->text(0));
  }
  myValidator = new QAD_FileValidator(this);
  
}

/*!
Destructor
*/
QAD_FileDlg::~QAD_FileDlg() 
{
}

/*!
Sets validator for file names to open/save
Deletes previous validator
*/
void QAD_FileDlg::setValidator( QAD_FileValidator* v )
{
  if (myValidator)
    delete myValidator;
  myValidator = v;
}

/*!
Returns the selected file
*/
QString QAD_FileDlg::selectedFile() const
{
  return mySelectedFile;
}

/*!
Returns 'true' if this is 'Open File' dialog 
and 'false' if 'Save File' dialog
*/
bool QAD_FileDlg::isOpenDlg() const
{
  return myOpen;
}

/*!
Closes this dialog and sets the return code to 'Accepted'
if the selected name is valid ( see 'acceptData()' )
*/
void QAD_FileDlg::accept()
{
//  mySelectedFile = QFileDialog::selectedFile().simplifyWhiteSpace(); //VSR- 06/12/02
  if ( mode() != ExistingFiles ) {
    mySelectedFile = QFileDialogP::selectedFile(); //VSR+ 06/12/02
    addExtension();
  }
//  mySelectedFile = mySelectedFile.simplifyWhiteSpace(); //VSR- 06/12/02

  /* Qt 2.2.2 BUG: accept() is called twice if you validate 
  the selected file name by pressing 'Return' key in file 
  name editor but this name is not acceptable for acceptData()
  */
  if ( acceptData() ) {
    myLastVisitedPath = dirPath();
    QFileDialogP::accept();        
  }
}

/*!
Closes this dialog and sets the return code to 'Rejected'    
*/
void QAD_FileDlg::reject()
{
  mySelectedFile = QString::null;
  QFileDialogP::reject();        
}

/*!
Returns 'true' if selected file is valid.
The validity is checked by a file validator, 
if there is no validator the file is always
considered as valid    
*/
bool QAD_FileDlg::acceptData()
{    
  if ( myValidator )
  {
    if ( isOpenDlg() )
      if ( mode() == ExistingFiles ) {
	QStringList fileNames = selectedFiles();
	for ( int i = 0; i < fileNames.count(); i++ ) {
	  if ( !myValidator->canOpen( fileNames[i] ) )
	    return false;
	}
	return true;
      }
      else {
	return myValidator->canOpen( selectedFile() );
      }
    else 
      return myValidator->canSave( selectedFile() );
  }
  return true;
}

/*!
Adds an extension to the selected file name
if the file has not it.
The extension is extracted from the active filter.
*/
void QAD_FileDlg::addExtension()
{
//  mySelectedFile.stripWhiteSpace();//VSR- 06/12/02
//  if ( mySelectedFile.isEmpty() )//VSR- 06/12/02
  if ( mySelectedFile.stripWhiteSpace().isEmpty() )//VSR+ 06/12/02
    return;

//  if ( QAD_Tools::getFileExtensionFromPath( mySelectedFile ).isEmpty() ) //VSR- 06/12/02
//ota :   16/12/03  if ( QAD_Tools::getFileExtensionFromPath( mySelectedFile ).isEmpty() ) //VSR+ 06/12/02
//  {

#if QT_VERSION < 0x030000
    QRegExp r( QString::fromLatin1("([a-zA-Z0-9.*? +;#]*)$") );
    int len, index = r.match( selectedFilter(), 0, &len );
#else
    QRegExp r( QString::fromLatin1("\\([a-zA-Z0-9.*? +;#]*\\)$") );
    int index = r.search(selectedFilter());
#endif
    if ( index >= 0 ) 
    {            
#if QT_VERSION < 0x030000
//      QString wildcard = selectedFilter().mid( index + 1, len-2 ); //VSR- 06/12/02
      QString wildcard = selectedFilter().mid( index + 1, len-2 ).stripWhiteSpace(); //VSR+ 06/12/02
#else
//      QString wildcard = selectedFilter().mid( index + 1, r.matchedLength()-2 ); //VSR- 06/12/02
      QString wildcard = selectedFilter().mid( index + 1, r.matchedLength()-2 ).stripWhiteSpace(); //VSR+ 06/12/02
#endif
      if ( mySelectedFile[mySelectedFile.length() - 1] == '.')
	//if the file name ends with the point remove it
	mySelectedFile.truncate(mySelectedFile.length() - 1);
      QString anExt = "." + QAD_Tools::getFileExtensionFromPath( mySelectedFile ).stripWhiteSpace();
      // From the filters list make a pattern to validate a file extension
      // Due to transformations from the filter list (*.txt *.*xx *.c++ QAD*.* ) we 
      // will have the pattern (\.txt|\..*xx|\.c\+\+|\..*) (as we validate extension only we remove
      // stay extension mask only in the pattern
      QString aPattern(wildcard);
      QRegExp anExtRExp("("+aPattern.replace(QRegExp("(^| )[0-9a-zA-Z*_?]*\\."), " \\.").
			stripWhiteSpace().replace(QRegExp("\\s+"), "|").
			replace(QRegExp("[*]"),".*").replace(QRegExp("[+]"),"\\+") + ")");
      
      if ( anExtRExp.match(anExt) == -1 ) //if a selected file extension does not match to filter's list
	{ //remove a point if it is at the word end
	  if (anExt[ anExt.length() - 1 ] == '.')  anExt.truncate( anExt.length() - 1 );
	  index = wildcard.findRev( '.' );    
	  if ( index >= 0 ) 
	    mySelectedFile += wildcard.mid( index ); //add the extension
	}
    }
  //  }
}

/*!
  Processes selection : tries to set given path or filename as selection
*/
bool QAD_FileDlg::processPath( const QString& path )
{
  if ( !path.isNull() ) {
    QFileInfo fi( path );
    if ( fi.exists() ) {
      if ( fi.isFile() )
	setSelection( path );
      else if ( fi.isDir() )
	setDir( path );
      return true;
    }
    else {
      if ( QFileInfo( fi.dirPath() ).exists() ) {
	setDir( fi.dirPath() );
	setSelection( path );
	return true;
      }
    }
  }
  return false;
}
/*!
  Called when user selects item from "Quick Dir" combo box
*/
void QAD_FileDlg::quickDir(const QString& dirPath)
{
  if ( !QDir(dirPath).exists() ) {
    QAD_MessageBox::error1(this, 
			   tr("ERR_ERROR"),
			   tr("ERR_DIR_NOT_EXIST").arg(dirPath), 
			   tr("BUT_OK"));
    
  }
  else {
    processPath(dirPath);
  }
}

/*!
  Returns the file name for Open/Save [ static ]
*/
QString QAD_FileDlg::getFileName( QWidget*           parent, 
				  const QString&     initial, 
				  const QStringList& filters, 
				  const QString&     caption,
				  bool               open,
				  bool               showQuickDir, 
				  QAD_FileValidator* validator )
{            
  QAD_FileDlg* fd = new QAD_FileDlg( parent, open, showQuickDir, true );    
  if ( !caption.isEmpty() )
    fd->setCaption( caption );
  if ( !initial.isEmpty() ) { 
    fd->processPath( initial ); // VSR 24/03/03 check for existing of directory has been added to avoid QFileDialog's bug
  }

  fd->setFilters( filters );

  if ( validator )
    fd->setValidator( validator );
  fd->exec();
  QString filename = fd->selectedFile();
  delete fd;
  qApp->processEvents();
  return filename;
}


/*!
  Returns the list of files to be opened [ static ]
*/
QStringList QAD_FileDlg::getOpenFileNames( QWidget*           parent, 
					   const QString&     initial, 
					   const QStringList& filters, 
					   const QString&     caption,
					   bool               showQuickDir, 
					   QAD_FileValidator* validator )
{            
  QAD_FileDlg* fd = new QAD_FileDlg( parent, true, showQuickDir, true );    
  fd->setMode( ExistingFiles );     
  if ( !caption.isEmpty() )
    fd->setCaption( caption );
  if ( !initial.isEmpty() ) { 
    fd->processPath( initial ); // VSR 24/03/03 check for existing of directory has been added to avoid QFileDialog's bug
  }
  fd->setFilters( filters );        
  if ( validator )
    fd->setValidator( validator );
  fd->exec();
  QStringList filenames = fd->selectedFiles();
  delete fd;
  qApp->processEvents();
  return filenames;
}

/*!
  Existing directory selection dialog [ static ]
*/
QString QAD_FileDlg::getExistingDirectory ( QWidget*       parent,
					    const QString& initial,
					    const QString& caption, 
					    bool           showQuickDir )
{
  QAD_FileDlg* fd = new QAD_FileDlg( parent, true, showQuickDir, true);
  if ( !caption.isEmpty() )
    fd->setCaption( caption );
  if ( !initial.isEmpty() ) {
    fd->processPath( initial ); // VSR 24/03/03 check for existing of directory has been added to avoid QFileDialog's bug
  }
  fd->setMode( DirectoryOnly );
  fd->setFilters(tr("DIRECTORIES_FILTER"));

  fd->exec();
  QString dirname = fd->selectedFile();
  delete fd;
  qApp->processEvents();
  return dirname;
}

