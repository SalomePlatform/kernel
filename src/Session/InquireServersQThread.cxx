using namespace std;
// File:	InquireServersQThread.cxx
// Created:	Mon Oct 21 17:26:42 2002
// Author:      Vasily RUSYAEV <vrv@nnov.matra-dtv.fr>
// Project      SALOME

#include "InquireServersQThread.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qabstractlayout.h> 
#include <qlayout.h>
#include <qevent.h> 
#include <qfont.h> 
#include <qmessagebox.h> 
#include <qdir.h>
#include <qfileinfo.h>
#include <qstringlist.h>

//VRV: porting on Qt 3.0.5
#if QT_VERSION >= 0x030005
#include <qdesktopwidget.h> 
#endif
//VRV: porting on Qt 3.0.5

#include <qsize.h> 

#include <SALOMEconfig.h>

#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOME_NamingService.hxx"
#include "utilities.h"
#include "OpUtil.hxx"

#include CORBA_CLIENT_HEADER(SALOME_Session)
#include CORBA_CLIENT_HEADER(SALOME_Registry)
#include CORBA_CLIENT_HEADER(SALOMEDS)
#include CORBA_CLIENT_HEADER(SALOME_ModuleCatalog)
#include CORBA_CLIENT_HEADER(SALOME_Component)

#define MARGIN_SIZE  5
#define SPACING_SIZE 3

static QString findFile( QString filename );
static QString addSlash( const QString& path );

InquireServersGUI::InquireServersGUI()
     : QVBox(0, "SFA splash", Qt::WDestructiveClose | Qt::WStyle_Customize | Qt::WStyle_NoBorder )
{
  myThread = new InquireServersQThread(this);

  // 1. Polish the appearance
  setMargin( MARGIN_SIZE );
  setSpacing( SPACING_SIZE );
  setFrameStyle( QFrame::Plain | QFrame::Box );
  setLineWidth( 2 );
  setMinimumSize( 200, 150 );

  // 2. Splash image
  QFrame* frm = new QFrame( this );
  frm->setFrameStyle( QFrame::Box | QFrame::Raised );
  QHBoxLayout* frmLayout = new QHBoxLayout( frm );
  frmLayout->setMargin( MARGIN_SIZE );
  QLabel* splash = 0;
  splash = new QLabel( frm, "splash" );
  frmLayout->addWidget( splash );
  // setting pixmap
  QString path = findFile( "Application-Splash.png" );
  splash->setPixmap( QPixmap( path )  );
  
  // 3. Progress bar
  myPrgBar = new QProgressBar( this, "QProgressBar" );
  myPrgBar->setFixedWidth( splash->pixmap()->isNull() ? 180 : splash->sizeHint().width() );
  //Sets the total number of steps . 
  myPrgBar->setTotalSteps ( myThread->getInquiredServers() );
  myPrgBar->setProgress( 0 );

  // 4. Info label
  QWidget* aWgt1 = new QWidget( this );
  QHBoxLayout* aHBoxLayout1 = new QHBoxLayout( aWgt1 );
  myLabel = new QLabel( tr( "Loading:" ), aWgt1 );
  myLabel->setFixedWidth( splash->pixmap()->isNull() ? 180 : splash->sizeHint().width() );
  myLabel->setAlignment( AlignLeft );
  QFont theFont = myLabel->font();
  theFont.setBold(true);
  myLabel->setFont( theFont );
  aHBoxLayout1->addItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
  aHBoxLayout1->addWidget( myLabel );
  aHBoxLayout1->addItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum ) );

  // 5. <Cancel> button
  QWidget* aWgt = new QWidget( this );
  QHBoxLayout* aHBoxLayout = new QHBoxLayout( aWgt );
  QPushButton* myCancelBtn = new QPushButton( tr( "Cancel" ), aWgt );
  connect( myCancelBtn, SIGNAL( clicked() ), this, SLOT( ClickOnCancel() ) ) ;
  aHBoxLayout->addItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum )  );
  aHBoxLayout->addWidget( myCancelBtn );
  aHBoxLayout->addItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum )  );

  //Center widget
#if QT_VERSION >= 0x030005
  QDesktopWidget *d = QApplication::desktop();
#else
  QWidget *d = QApplication::desktop();
#endif
//VRV: porting on Qt 3.0.5

  int w = d->width();         // returns desktop width
  int h = d->height();        // returns desktop height
  QSize mySize = sizeHint (); // returns widget size
  int Xc = ( w - mySize.width() )  / 2;
  int Yc = ( h - mySize.height() ) / 2;
  move( Xc, Yc );

  myThread->start();
}

InquireServersGUI::~InquireServersGUI()
{
  delete myThread;
}

void InquireServersGUI::getArgs( int& _argc, char *** _argv)
{
  _argc = qApp->argc();
  *_argv = qApp->argv();
}

//=================================================================================
// function : ClickOnCancel()
// purpose  : cancel loading of SALOME
//=================================================================================
void InquireServersGUI::ClickOnCancel()
{
  //it's necessary to stop asking servers
  myThread->stop();
  //Also we should send QCloseEvent in order to close this widget (and remove from screen) 
  //QThread::postEvent ( this, new QCloseEvent() );
  qApp->exit(1);
}

void InquireServersGUI::closeEvent ( QCloseEvent * pe)
{
  //default implementation calls e->accept(), which hides this widget. 
  //See the QCloseEvent documentation for more details.
  pe->accept();
  QApplication::flushX ();
  QApplication::syncX ();
  qApp->exit();
}

void InquireServersGUI::customEvent( QCustomEvent* pe )
{
  switch( pe->type() )
    {
        case InquireEvent::ProgressEvent:
        {
            int* value = ( int* )(( InquireEvent*)pe)->data();
            myPrgBar->setProgress( *value );
            break;
        }
        case InquireEvent::ProgressEventLabel:
        {
            QString* myString = ( QString* )(( InquireEvent*)pe)->data();
            myLabel->setText( *myString );
            break;
        }
        case InquireEvent::ProgressEventError:
        {
            QString* myErrDesc = ( QString* )(( InquireEvent*)pe)->data();
 	    QString  appName = "SALOME Professional";
 	    QString  error = "An internal error occurred.\n"+ *myErrDesc + "\n";
	    QMessageBox myMsgBox(appName,error,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton,
					QMessageBox::NoButton,0,"MY",TRUE,WStyle_DialogBorder|WStyle_StaysOnTop);
	    myMsgBox.exec();
	    ClickOnCancel();
            break;
        }
      default:
	{
	  ;
	}
    }
}

int InquireServersGUI::getExitStatus()
{
  myThread->getExitStatus();
}

InquireServersQThread::InquireServersQThread( InquireServersGUI* r )
     : receiver(r),  myExitStatus(0)
{
  char* cenv;

  IsChecking = true;
  myServersCount = 8;
  //how many times we should repeat attempts to get response from all needed for launching SALOME servers
  myRepeat = 30; // default value, user can change it by setting CSF_RepeatServerRequest env.variable
  cenv = getenv( "CSF_RepeatServerRequest" );
  if ( cenv ) {
    int val = atoi( cenv );
    if ( val > 0 )
      myRepeat = val;
  }
  //define delay time between two attempts
  myDelay = 1000000; // 1 second
  QString str = "Loading: ";
  myMessages[0] = "Checking naming service...";
  myMessages[1] = str + "SALOME_Registry_Server" + "...";
  myMessages[2] = str + "SALOMEDS_Server" + "...";
  myMessages[3] = str + "SALOME_ModuleCatalog_Server" + "...";
  myMessages[4] = str + "SALOME_Session_Server" + "...";
  myMessages[5] = str + "SALOME_Container FactoryServer" + "...";
  myMessages[6] = str + "SALOME_ContainerPy.py FactoryServerPy" + "...";
  myMessages[7] = str + "SALOME_Container SuperVisionContainer" + "...";

  r->getArgs( _argc, &_argv);

}

void InquireServersQThread::run()
{
while (IsChecking)
  {
    for (int i=1; i<=myServersCount; i++)
      {
	QString *message = new QString(myMessages[i-1]);
	QThread::postEvent( receiver, new InquireEvent( ( QEvent::Type )InquireEvent::ProgressEventLabel, message ) );
	QThread::usleep(200000);
	QString *errMsg;
	bool result = AskServer(i,&errMsg);
	if (result)
	  {
	    QThread::postEvent( receiver, new InquireEvent( ( QEvent::Type )InquireEvent::ProgressEvent, new int( i ) ) );
	    if (i==myServersCount)
	      {
		IsChecking = false;
		//myExitStatus should be 0 because all servers exist and work
		myExitStatus = 0;
		//we should send QCloseEvent in order to close this widget (and remove from screen) 
		QThread::postEvent ( receiver , new QCloseEvent() );
	      }
	  }
	else
	  {
	    QThread::postEvent( receiver, new InquireEvent( ( QEvent::Type )InquireEvent::ProgressEventError, errMsg ) );
	    //myExitStatus should be 1 because we didn't receive response from server
	    myExitStatus = 1;
	    return;
	  }
      }
  }
}

bool InquireServersQThread::AskServer(int iteration, QString ** errMessage)
{
  ASSERT(iteration<=myServersCount);
  //will be set true if we get response from server
  bool IsPassed = false;
  QString errDescription;
  switch (iteration)
    {
    case 1:
      //First checking - existence of Naming Service
      for (int i = myRepeat; i ; i--)
	{
	  try
	    {
	      CORBA::ORB_var orb = CORBA::ORB_init(_argc,_argv) ;
	      CORBA::Object_var obj = orb->resolve_initial_references("NameService");
	      CosNaming::NamingContext_var _root_context = CosNaming::NamingContext::_narrow(obj);
	      if (CORBA::is_nil(_root_context))
		continue;
	      else
		IsPassed = true;
	      break;
	    }
	  catch(CORBA::COMM_FAILURE&)
	    {
	      MESSAGE("CORBA::COMM_FAILURE: unable to contact the naming service");
	    }
	  catch(...)
	    {
	      MESSAGE("Unknown Exception: unable to contact the naming service");
	    }
	  QThread::usleep(myDelay);
	}
      if (!IsPassed)
	*errMessage = new QString("unable to contact the naming service");
      break;
    case 2:
      //checking - existence of SALOME_Registry_Server
    case 3:
      //checking - existence of SALOMEDS_Server
    case 4:
      //checking - existence of SALOME_ModuleCatalog_Server
    case 5:
      //checking - existence of SALOME_Session_Server
    case 6:
      //checking - existence of SALOME_Container FactoryServer
    case 7:
      //checking - existence of SALOME_ContainerPy.py FactoryServerPy
    case 8:
      //checking - existence of SALOME_Container SuperVisionContainer
      IsPassed = pingServer(iteration, errDescription);
      if (!IsPassed)
	*errMessage = new QString(errDescription);
      break;
    }
return IsPassed;
}

bool InquireServersQThread::pingServer(int iteration, QString& errMessage)
{
  ASSERT(iteration<=myServersCount);
  bool result = false;
  QString errorDescr;
  for (int i = myRepeat; i ; i--)
    {
      try
	{
	  CORBA::ORB_var orb = CORBA::ORB_init(_argc,_argv) ;
	  SALOME_NamingService &NS = *SINGLETON_<SALOME_NamingService>::Instance() ;
	  ASSERT(SINGLETON_<SALOME_NamingService>::IsAlreadyExisting()) ;
	  NS.init_orb( orb ) ;
	  switch (iteration)
	    {
	    case 2:
	      {
		CORBA::Object_var obj = NS.Resolve("/Registry");
		Registry::Components_var registry = Registry::Components::_narrow(obj) ;
		if (!CORBA::is_nil(registry))
		  {
		    MESSAGE("/Registry is found");
		    registry->ping();
		    result = true;
		    MESSAGE("Registry was activated");
		    return result;
		  }
	      }
	      break;
	    case 3:
	      {
		CORBA::Object_var obj = NS.Resolve("/myStudyManager");
		SALOMEDS::StudyManager_var studyManager = SALOMEDS::StudyManager::_narrow(obj) ;
		if (!CORBA::is_nil(studyManager))
		  {
		    MESSAGE("/myStudyManager is found");
		    studyManager->ping();
		    result = true;
		    MESSAGE("StudyManager was activated");
		    return result;
		  }
	      }
	      break;
	    case 4:
	      {
		CORBA::Object_var obj = NS.Resolve("Kernel/ModulCatalog");
		SALOME_ModuleCatalog::ModuleCatalog_var catalog = SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj) ;
		if (!CORBA::is_nil(catalog))
		  {
		    MESSAGE("/Kernel/ModulCatalog is found");
		    catalog->ping();
		    result = true;
		    MESSAGE("ModuleCatalog was activated");
		    return result;
		  }
	      }
	      break;
	    case 5:
	      {
		CORBA::Object_var obj = NS.Resolve("Kernel/Session");
		SALOME::Session_var session = SALOME::Session::_narrow(obj) ;
		if (!CORBA::is_nil(session))
		  {
		    MESSAGE("/Kernel/Session is found");
		    session->ping();
		    result = true;
		    MESSAGE("SALOME_Session was activated");
		    return result;
		  }
	      }
	      break;
	    case 6:
	      {
		string hostname = GetHostname();
		string containerName = "/Containers/";
		containerName += hostname;
		containerName += "/FactoryServer";

		CORBA::Object_var obj = NS.Resolve(containerName.c_str());
		Engines::Container_var FScontainer = Engines::Container::_narrow(obj) ;
		if (!CORBA::is_nil(FScontainer))
		  {
		    FScontainer->ping();
		    result = true;
		    MESSAGE("FactoryServer container was activated");
		    return result;
		  }
	      }
	      break;
	    case 7:
	      {
		string hostname = GetHostname();
		string containerName = "/Containers/";
		containerName += hostname;
		containerName += "/FactoryServerPy";
		
		CORBA::Object_var obj = NS.Resolve(containerName.c_str());
		Engines::Container_var FSPcontainer = Engines::Container::_narrow(obj) ;
		if (!CORBA::is_nil(FSPcontainer))
		  {
		    FSPcontainer->ping();
		    result = true;
		    MESSAGE("FactoryServerPy container was activated");
		    return result;
		  }
	      }
	      break;
	    case 8:
	      {
		string hostname = GetHostname();
		string containerName = "/Containers/";
		containerName += hostname;
		containerName += "/SuperVisionContainer";
		
		CORBA::Object_var obj = NS.Resolve(containerName.c_str());
		Engines::Container_var SVcontainer = Engines::Container::_narrow(obj) ;
		if (!CORBA::is_nil(SVcontainer))
		  {
		    SVcontainer->ping();
		    result = true;
		    MESSAGE("SuperVisionContainer container was activated");
		    return result;
		  }
	      }
	      break;
	    }
	 }
      catch (ServiceUnreachable&)
	{
	  MESSAGE("Caught exception: Naming Service Unreachable");
	  errorDescr = "Caught exception: Naming Service Unreachable";
	}
      catch (CORBA::COMM_FAILURE&)
	{
	  MESSAGE("Caught CORBA::SystemException CommFailure.");
	  errorDescr = "Caught CORBA::SystemException CommFailure";
	}
      catch (CORBA::SystemException&)
	{
	  MESSAGE("Caught CORBA::SystemException.");
	  errorDescr = "Caught CORBA::SystemException";
	}
      catch (CORBA::Exception&)
	{
	  MESSAGE("Caught CORBA::Exception.");
	  errorDescr = "Caught CORBA::Exception";
	}
      catch (...)
	{
	  MESSAGE("Caught unknown exception.");
	  errorDescr = "Caught unknown exception";
	}
      QThread::usleep(myDelay);
    }
  if (!result)
    {
      QString serverName;
      switch (iteration)
	{
	case 2:
	  serverName = "SALOME_Registry_Server is not loaded. ";
	  break;
	case 3:
	  serverName = "SALOMEDS_Server is not loaded. ";
	  break;
	case 4:
	  serverName = "SALOME_ModuleCatalog_Server is not loaded. ";
	  break;
	case 5:
	  serverName = "SALOME_Session_Server is not loaded. ";
	  break;
	case 6:
	  serverName = "SALOME_Container FactoryServer is not loaded. ";
	  break;
	case 7:
	  serverName = "SALOME_ContainerPy.py FactoryServerPy is not loaded. ";
	  break;
	case 8:
	  serverName = "SALOME_Container SuperVisionContainer is not loaded. ";
	  break;
	}
      errMessage = serverName + errorDescr;
    }
  return result;
}

static const char* SEPARATOR    = ":";

QString findFile( QString filename )
{
  QString dir;
  char* cenv;
  
  // Try ${KERNEL_ROOT_DIR}/share/salome/resources directory
  cenv = getenv( "KERNEL_ROOT_DIR" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      dir = addSlash(dir) ;
      dir = dir + "share" ;
      dir = addSlash(dir) ;
      dir = dir + "salome" ;
      dir = addSlash(dir) ;
      dir = dir + "resources" ;
      dir = addSlash(dir) ;
      QFileInfo fileInfo( dir + filename );
      if ( fileInfo.isFile() && fileInfo.exists() )
	return fileInfo.filePath();
    }
  }
  // Try CSF_ResourcesDefaults env.var directory ( or directory list )
  cenv = getenv( "CSF_ResourcesDefaults" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      QStringList dirList = QStringList::split( SEPARATOR, dir, false ); // skip empty entries
      for ( int i = 0; i < dirList.count(); i++ ) {
	QFileInfo fileInfo( addSlash( dirList[ i ] ) + filename );
	if ( fileInfo.isFile() && fileInfo.exists() )
	  return fileInfo.filePath();
      }
    }
  }
  // Try ${HOME}/.salome/resources directory
  cenv = getenv( "HOME" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      dir = addSlash(dir) ;
      dir = dir + ".salome" ;
      dir = addSlash(dir) ;
      dir = dir + "resources" ;
      dir = addSlash(dir) ;
      QFileInfo fileInfo( dir + filename );
      if ( fileInfo.isFile() && fileInfo.exists() )
	return fileInfo.filePath();
    }
  }
  // Try ${SALOME_SITE_DIR}/share/salome/resources directory
  cenv = getenv( "SALOME_SITE_DIR" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      dir = addSlash(dir) ;
      dir = dir + "share" ;
      dir = addSlash(dir) ;
      dir = dir + "salome" ;
      dir = addSlash(dir) ;
      dir = dir + "resources" ;
      dir = addSlash(dir) ;
      QFileInfo fileInfo( dir + filename );
      if ( fileInfo.isFile() && fileInfo.exists() )
	return fileInfo.filePath();
    }
  }
  // Try ${SALOME_ROOT_DIR}/share/salome/resources directory
  cenv = getenv( "SALOME_ROOT_DIR" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      dir = addSlash(dir) ;
      dir = dir + "share" ;
      dir = addSlash(dir) ;
      dir = dir + "salome" ;
      dir = addSlash(dir) ;
      dir = dir + "resources" ;
      dir = addSlash(dir) ;
      QFileInfo fileInfo( dir + filename );
      if ( fileInfo.isFile() && fileInfo.exists() )
	return fileInfo.filePath();
    }
  }
  return filename;
}
QString addSlash( const QString& path )
{
  if (!path.isNull()) {
#ifdef WNT
    QChar slash ('\\');
#else
    QChar slash ('/');
#endif
    if ( path.at(path.length()-1) != slash )
      return path + slash;
  }
  return path;
}
