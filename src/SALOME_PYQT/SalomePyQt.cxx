//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SalomePyQt.cxx
//  Module : SALOME

#include "SalomePyQt.h"

#include <qapplication.h>
#include <qmenubar.h>

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_FileDlg.h"
#include "QAD_ViewFrame.h"
#include "QAD_RightFrame.h"
#include "QAD_Tools.h"

#include "QAD_Config.h"
#include "QAD_Settings.h"

//====================================================================================
// SalomePyQt_Selection class.
//====================================================================================
static QMap<QAD_Study*, SalomePyQt_Selection*> SelMap;

/*!
  SalomePyQt_Selection::GetSelection
  Creates or finds the selection object (one per study).
*/
SalomePyQt_Selection* SalomePyQt_Selection::GetSelection( QAD_Study* study )
{
  SalomePyQt_Selection* sel = 0;
  if ( study && SelMap.find( study ) != SelMap.end() )
    sel = SelMap[ study ];
  else 
    sel = SelMap[ study ] = new SalomePyQt_Selection( study );
  return sel;
}

/*!
  SalomePyQt_Selection::SalomePyQt_Selection
  Selection constructor.
*/
SalomePyQt_Selection::SalomePyQt_Selection( QObject* p ) :
  QObject( p ), myStudy( 0 ), mySelection( 0 )
{
  myStudy = dynamic_cast<QAD_Study*>( p );
  if ( myStudy ) {
    mySelection = SALOME_Selection::Selection( myStudy->getSelection() );
    if ( mySelection ) {
      connect( mySelection, SIGNAL( currentSelectionChanged() ), 
	       this,        SIGNAL( currentSelectionChanged() ) );
      connect( mySelection, SIGNAL( destroyed() ),        
	       this,        SLOT  ( onSelectionDestroyed() ) );
      connect( myStudy,     SIGNAL( selectionModified( QAD_Study* ) ),        
	       this,        SLOT  ( onSelectionModified() ) );
    }
  }
}
/*!
  SalomePyQt_Selection::~SalomePyQt_Selection
  Selection destructor. Removes selection object from the map.
*/
SalomePyQt_Selection::~SalomePyQt_Selection()
{
  if ( myStudy && SelMap.find( myStudy ) != SelMap.end() )
    SelMap.remove( myStudy );
}

/*!
  SalomePyQt_Selection::onSelectionDestroyed
  Watches for the selection destroying (e.g. when study is closed).
*/
void SalomePyQt_Selection::onSelectionDestroyed()
{
  mySelection = 0;
}

/*!
  SalomePyQt_Selection::onSelectionModified
  Updates and reconnect selection when it is changed (e.g. when study is saved).
*/
void SalomePyQt_Selection::onSelectionModified()
{
  if ( mySelection ) {
    disconnect( mySelection, SIGNAL( currentSelectionChanged() ), 
	        this,        SIGNAL( currentSelectionChanged() ) );
    disconnect( mySelection, SIGNAL( destroyed() ),        
	        this,        SLOT  ( onSelectionDestroyed() ) );
  }
  
  mySelection = myStudy ? SALOME_Selection::Selection( myStudy->getSelection() ) : 0;
    
  if ( mySelection ) {
    connect( mySelection, SIGNAL( currentSelectionChanged() ), 
	     this,        SIGNAL( currentSelectionChanged() ) );
    connect( mySelection, SIGNAL( destroyed() ),        
	     this,        SLOT  ( onSelectionDestroyed() ) );
  }
}

/*!
  SalomePyQt_Selection::Clear
  Clears the selection.
*/
void SalomePyQt_Selection::Clear()
{
  if ( mySelection ) mySelection->Clear();
}

/*!
  SalomePyQt_Selection::ClearIObjects
  Clears the selection and emits the signal.
*/
void SalomePyQt_Selection::ClearIObjects()
{
  if ( mySelection ) mySelection->ClearIObjects();
}

/*!
  SalomePyQt_Selection::ClearFilters
  Removes all selection filters.
*/
void SalomePyQt_Selection::ClearFilters()
{
  if ( mySelection ) mySelection->ClearFilters();
}

//====================================================================================
// SalomePyQt class
//====================================================================================

using namespace std;

QWidget* SalomePyQt::getDesktop()
{
  return (QWidget*)(QAD_Application::getDesktop());
}

QWorkspace* SalomePyQt::getMainFrame()
{
  return (QWorkspace*)(QAD_Application::getDesktop()->getMainFrame());
}

QMenuBar* SalomePyQt::getMainMenuBar() 
{
  return (QMenuBar*)(QAD_Application::getDesktop()->getMainMenuBar());
}

int SalomePyQt::getStudyId()
{
  return QAD_Application::getDesktop()->getActiveApp()->getActiveStudy()->getStudyId();
}

SalomePyQt_Selection* SalomePyQt::getSelection()
{
  return SalomePyQt_Selection::GetSelection(QAD_Application::getDesktop()->getActiveApp()->getActiveStudy());
}

void SalomePyQt::putInfo( const QString& msg )
{
  QAD_Application::getDesktop()->putInfo(msg);
}

void SalomePyQt::putInfo( const QString& msg, int ms )
{
  QAD_Application::getDesktop()->putInfo(msg, ms);
}

void SalomePyQt::updateObjBrowser( int studyId, bool updateSelection)
{  
  QList<QAD_Study>& studies = QAD_Application::getDesktop()->getActiveApp()->getStudies();
  for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
    if ( study->getStudyId() == studyId ) {
      study->updateObjBrowser( updateSelection );
      break;
    }
  }
}

const QString& SalomePyQt::getActiveComponent()
{
  return QAD_Application::getDesktop()->getActiveComponent();
}

void SalomePyQt::addStringSetting(QString _name, QString _value, bool _autoValue)
{
  QAD_CONFIG->addSetting(_name, _value, _autoValue);
}

void SalomePyQt::addIntSetting(QString _name, int _value, bool _autoValue)
{
  QAD_CONFIG->addSetting(_name, _value, _autoValue);
}

void SalomePyQt::addDoubleSetting(QString _name, double _value, bool _autoValue)
{
  QAD_CONFIG->addSetting(_name, _value, _autoValue);
}

bool SalomePyQt::removeSettings(QString name)
{
  return QAD_CONFIG->removeSettings( name );
}

QString SalomePyQt::getSetting(QString name)
{
  return QAD_CONFIG->getSetting(name);
}

QString SalomePyQt::getFileName(QWidget*           parent, 
				const QString&     initial, 
				const QStringList& filters, 
				const QString&     caption,
				bool               open)
{
  return QAD_FileDlg::getFileName(parent, initial, filters, caption, open);
}

QStringList SalomePyQt::getOpenFileNames(QWidget*           parent, 
					 const QString&     initial, 
					 const QStringList& filters, 
					 const QString&     caption)
{
  return QAD_FileDlg::getOpenFileNames(parent, initial, filters, caption);
}

QString SalomePyQt::getExistingDirectory(QWidget*       parent,
					 const QString& initial,
					 const QString& caption)
{
  return QAD_FileDlg::getExistingDirectory(parent, initial, caption);
}

void SalomePyQt::helpContext(const QString& source, const QString& context) {
  //QAD_Application::getDesktop()->helpContext(source, context);
}

bool SalomePyQt::dumpView(const QString& filename)
{
  QAD_Study* activeStudy = QAD_Application::getDesktop()->getActiveApp()->getActiveStudy();
  if ( !activeStudy )
    return false;
  QAD_ViewFrame* activeViewFrame = activeStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame();
  if ( !activeViewFrame )
    return false;
  if ( !activeViewFrame->getViewWidget() )
    return false;

  qApp->processEvents();
  QPixmap px = QPixmap::grabWindow( activeViewFrame->getViewWidget()->winId() );
  if ( !filename.isNull() ) {
    QString fmt = QAD_Tools::getFileExtensionFromPath( filename ).upper();
    if ( fmt.isEmpty() )
      fmt = QString( "PNG" ); // default format
    if ( fmt == "JPG" )
      fmt = "JPEG";
    bool bOk = px.save( filename, fmt.latin1() );
    return bOk;
  }
  return false;
}

QPopupMenu* SalomePyQt::getPopupMenu( const MenuName menu )
{
  QMenuBar* mainMenu = QAD_Application::getDesktop()->getMainMenuBar();
  if ( mainMenu->findItem( menu ) ) {
    return mainMenu->findItem( menu )->popup();
  }
  return 0;
}
