using namespace std;
//  File      : QAD_ResourceMgr.cxx
//  Created   : UI team, 22.10.00
//  Descr     : ResourceMgr QAD-based application
//  Author    : UI team
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

/*!
  \class QAD_ResourceMgr QAD_ResourceMgr.h
  \brief ResourceMgr QAD-based application.
*/

#include "QAD.h"
#include "QAD_Tools.h"
#include "QAD_MessageBox.h"
#include "QAD_ResourceMgr.h"

#include "utilities.h"

#include <qfile.h>
#include <stdlib.h>
#include <qtranslator.h>
#include <qapplication.h>
#include <qfileinfo.h>

#include <Standard.hxx>

/* configuration file */
static const char* CONFIG_FILE = "config";

/* config keys */
static const char* RES_DIR      = "res";            
static const char* RES_DOCS     = "docs";
static const char* RES_PIXMAPS  = "icons";
static const char* RES_STRINGS  = "strings";
static const char* RES_LANGUAGE = "language";

static const char* SEPARATOR    = ":";

/*!
    Constructor
*/
QAD_ResourceMgr::QAD_ResourceMgr() :
myRes( 5, false )
{    
    myRes.setAutoDelete( true );    
}

/*!
    Destructor
*/
QAD_ResourceMgr::~QAD_ResourceMgr()
{
     myRes.clear();
}

/*!
    Removes icons and messages from 'prefix'_msg_'lang'
    and 'prefix'_icons' files. Returns 'true' if OK. 
    Each application which has its own resources must
    have a unique 'prefix' ( prefix "QAD" is reserved )
*/
bool QAD_ResourceMgr::removeResources( const char* prefix )
{    
  return myRes.remove(prefix);
}

/*!
    Loads icons and messages from 'prefix'_msg_'lang'
    and 'prefix'_icons' files. Returns 'true' if OK. 
    Each application which has its own resources must
    have a unique 'prefix' ( prefix "QAD" is reserved )
*/
bool QAD_ResourceMgr::loadResources( const char* prefix, QString &msg )
{    
  bool allLoaded = true;    
  if ( !myRes[ prefix ] ) {
    QCString dir ;
    
    /*  We read the settings once and keep them. 
	The resources are loaded consequently from the end of directory list
	which ( see collectDirs() method description ). This allows to override
	resources when it is necessary.
    */
    
    ResourceSettings* settings = new ResourceSettings();
    StringDict& conf = settings->config();
    myRes.insert( prefix, settings );
    
    // settings->config().insert( RES_DIR, new QString( resDir ) );
    
    /* we search language definition : we read it in config file
       If not found, we use default : English
    */
    conf.insert( RES_LANGUAGE, new QString( "en" ) );
    
    /* Read configuration file */
    
    /* WE MUST HAVE ONE CONFIGURATION FILE FOR ALL SALOME !!!
       I DON'T KNOW WHERE READ IT AND SAVE ITS CONTENTS FOR ALL GUI
       ALL GUI HAS SAME LANGUAGE AND HAVE DEFAULT (en) IF SPECIFIED 
       IS NOT FOUND !!
    */
    QString resDirs = collectDirs( prefix );
    conf.insert( RES_DIR, new QString( resDirs ) );
    QString fileConfig = path( CONFIG_FILE, prefix, 0 ) ;
    //MESSAGE("QAD_ResourceMgr::loadresources : config : "<<fileConfig);
    if ( !fileConfig.isEmpty() ) {
      QFile configFile( fileConfig );
      if ( !configFile.exists() || !configFile.open( IO_ReadOnly ) ) {
	QString warnMsg;
	warnMsg.sprintf( "Cannot open configuration file: %s\nDefault settings will be used.",
			 configFile.name().latin1() );
	msg = warnMsg;
//	removeResources( prefix );
//	return false;
      } 
      else {     
	/* read 'config' file */
	const int MAX_LINE = 512;
	while ( !configFile.atEnd() ) {
	  QString line;
	  if ( configFile.readLine( line, MAX_LINE ) > 0 ) {
	    int index;
	    if ( ( index = line.find( "=" ) ) > 0 ) {
	      QString key = line.left(index).stripWhiteSpace();
	      QString value = line.mid( index+1 ).stripWhiteSpace();                    
	      conf.replace( key, new QString( value ) );
	    }
	  }
	}
	configFile.close();                        
      }
    }
    
    /* Load the resources */
    QString stFile( prefix );
    stFile = stFile + "_msg_" +  *( conf[ RES_LANGUAGE ] ) + ".qm" ;
    QString imagesFile( prefix );
    imagesFile = imagesFile + "_" + RES_PIXMAPS + ".qm";
    if ( conf[ RES_STRINGS ] && !conf[ RES_STRINGS ]->isEmpty() )
      stFile = QAD_Tools::addSlash( *conf[ RES_STRINGS ] ) + stFile;
    if ( conf[ RES_PIXMAPS ] && !conf[ RES_PIXMAPS ]->isEmpty() )
      imagesFile = QAD_Tools::addSlash( *conf[ RES_PIXMAPS ] ) + imagesFile;

    bool bLoadString = false;
    bool bLoadImages = false;

    QStringList dirList = QStringList::split( SEPARATOR, resDirs, false ); // skip empty entries
    for ( int i = dirList.count()-1; i >= 0; i-- ) {
      QString dir = dirList[ i ];
      QString fileString = QAD_Tools::addSlash( dir ) + stFile;
      QString fileImage  = QAD_Tools::addSlash( dir ) + imagesFile;
      
      if ( settings->load( fileString ) ) {
	bLoadString = true;
      }
      if ( settings->load( fileImage ) ) {
	bLoadImages = true;
      }
    }
    
    if ( !bLoadString ) {
      QString warnMsg;
      warnMsg.sprintf( "String resources for module %s not found.\n"
		       "Please, check your settings.", 
		       prefix );
      msg = warnMsg;
//      removeResources( prefix );
      return false;
    }
    if ( !bLoadImages ) {
      QString warnMsg;
      warnMsg.sprintf( "Icons resources for module %s not found.\n"
		       "Please, check your settings.", 
		       prefix );
      msg = warnMsg;
//      removeResources( prefix );
      return false;
    }
    allLoaded = bLoadString && bLoadImages;
  }
  return allLoaded;
}

/*!
  Returns language setting for the module 'prefix' ( e.g. "en" )
*/
QString QAD_ResourceMgr::language( const char* prefix ) const
{
  QString ret;
  ResourceSettings* rs = myRes[ prefix ];
  if ( rs ) 
    {
      StringDict& conf = rs->config();
      ret = *(conf[RES_LANGUAGE]);
    }
  return ret;
}

/*!
  Returns list of directories where resources can be located
  See collectDirs() method description for more detail
*/
QString QAD_ResourceMgr::resources( const char* prefix ) const
{
  QString ret;
  ResourceSettings* rs = myRes[ prefix ];
  if ( rs ) 
    {
      StringDict& conf = rs->config();
      ret = *(conf[RES_DIR]);
    }
  return ret;
}

/*!
  Collects list of directories, separated by ';' where resources for module 'prefix'
  can be situated
  The order is following : 
  - CSF_<prefix>Resources env.var directory ( or directory list )
  - CSF_ResourcesDefaults env.var directory ( or directory list )
  - ${HOME}/.salome/resources directory
  - ${SALOME_SITE_DIR}/share/salome/resources directory
  - ${SALOME_ROOT_DIR}/share/salome/resources directory
*/
QString QAD_ResourceMgr::collectDirs( const QString& prefix ) const
{
  QString dirList;
  QCString envVar( "CSF_" );
  QString dir;
  char* cenv;
  
  if ( !prefix.isEmpty() ) {
    envVar = prefix.latin1() + QCString( "_ROOT_DIR" );
    cenv = getenv( ( const char* ) envVar );
    if ( cenv ) {
      dir.sprintf( "%s", cenv );
      if ( !dir.isEmpty() ) {
	dir = QAD_Tools::addSlash(dir) ;
	dir = dir + "share" ;
	dir = QAD_Tools::addSlash(dir) ;
	dir = dir + "salome" ;
	dir = QAD_Tools::addSlash(dir) ;
	dir = dir + "resources" ;
	dir = QAD_Tools::addSlash(dir) ;
	dirList.append( dirList.isEmpty() ? dir : ( QString( SEPARATOR ) + dir ) );
      }
    }
  }

  // Try CSF_<prefix>Resources env.var directory ( or directory list )
  if ( !prefix.isEmpty() ) {
    envVar = QCString( "CSF_" ) + prefix.latin1() + QCString( "Resources" );
    cenv = getenv( ( const char* ) envVar );
    if ( cenv ) {
      dir.sprintf( "%s", cenv );
      if ( !dir.isEmpty() )
	dirList.append( dirList.isEmpty() ? dir : ( QString( SEPARATOR ) + dir ) );
    }
  }
  // Try CSF_ResourcesDefaults env.var directory ( or directory list )
  cenv = getenv( "CSF_ResourcesDefaults" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() )
      dirList.append( dirList.isEmpty() ? dir : ( QString( SEPARATOR ) + dir ) );
  }
  // Try ${HOME}/.salome/resources directory
  cenv = getenv( "HOME" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + ".salome" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "resources" ;
      dir = QAD_Tools::addSlash(dir) ;
      dirList.append( dirList.isEmpty() ? dir : ( QString( SEPARATOR ) + dir ) );
    }
  }
  // Try ${SALOME_SITE_DIR}/share/salome/resources directory
  cenv = getenv( "SALOME_SITE_DIR" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "share" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "resources" ;
      dir = QAD_Tools::addSlash(dir) ;
      dirList.append( dirList.isEmpty() ? dir : ( QString( SEPARATOR ) + dir ) );
    }
  }
  // Try ${SALOME_ROOT_DIR}/share/salome/resources directory
  cenv = getenv( "SALOME_ROOT_DIR" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "share" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "resources" ;
      dir = QAD_Tools::addSlash(dir) ;
      dirList.append( dirList.isEmpty() ? dir : ( QString( SEPARATOR ) + dir ) );
    }
  }

  // Try ${KERNEL_ROOT_DIR}/share/salome/resources directory
  cenv = getenv( "KERNEL_ROOT_DIR" );
  if ( cenv ) {
    dir.sprintf( "%s", cenv );
    if ( !dir.isEmpty() ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "share" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "resources" ;
      dir = QAD_Tools::addSlash(dir) ;
      dirList.append( dirList.isEmpty() ? dir : ( QString( SEPARATOR ) + dir ) );
    }
  }
  //MESSAGE("QAD_ResourceMgr::collectDirs : "<<dirList.latin1()) ;
  return dirList;
}

/*!
    Returns a directory where 'filename' is located (filename is relative 
    of the application identified by 'prefix' or empty string if file not found
    Search is processed in different location : see collectDirs() method description
*/
QString QAD_ResourceMgr::getFile( const QString& filename, const char* prefix ) const
{  
  QFileInfo fi( path( filename, prefix, 0 ) );
  if ( fi.isFile() && fi.exists() )
    return fi.dirPath();
  return QString();
}

/*!
    Returns a directory where 'filename' is located (filename is relative 
    of the application identified by 'prefix' or empty string if file not found
    Search is processed in different location : see collectDirs() method description
    The difference from above method that this function is used when resources 
    is not yet actually loaded by application.
*/
QString QAD_ResourceMgr::findFile( const QString& filename, const char* prefix ) const
{  
  QString resDirs = collectDirs( prefix );
  QStringList dirList = QStringList::split( SEPARATOR, resDirs, false ); // skip empty entries
  for ( int i = 0; i < dirList.count(); i++ ) {
    QString dir = dirList[ i ];
    QFileInfo fi( QAD_Tools::addSlash( dir ) + filename );
    if ( fi.isFile() && fi.exists() )
      return fi.dirPath();
  }
  return QString();
}

/*!
    Returns a path to file 'filename' (filename is relative 
    of the application identified by 'prefix' and subdirectory identified by 'key'
    or empty string if file not found.
    Search is processed in different location : see collectDirs() method description

    Returns a directory 'key' resource of the application 
    identified by 'prefix'    
*/
QString QAD_ResourceMgr::path( const QString& filename, const char* prefix, const char* key ) const
{   
  QString filePath;

  ResourceSettings* rs = myRes[ prefix ];
  if ( rs ) {
    StringDict& conf = rs->config();
    QString resDirs = QString( *( conf[ RES_DIR ] ) );
    if ( !resDirs.isEmpty() ) {
      //MESSAGE("QAD_ResourceMgr::resDirs : <"<<resDirs<<">") ;
      QStringList dirList = QStringList::split( SEPARATOR, resDirs, false ); // skip empty entries
      for ( int i = 0; i < dirList.count(); i++ ) {
	QString dir = dirList[ i ];
	dir = QAD_Tools::addSlash( dir );
	if ( key ) {
	  QString* where = conf[ key ];
	  if ( where )
	    dir = dir + QAD_Tools::addSlash( *where );
	}
	dir = dir + filename;
	QFileInfo fileInfo( dir );
	if ( fileInfo.isFile() && fileInfo.exists() ) {
	  filePath = fileInfo.filePath();
	  break;
	}
      }
    }
  }
  //MESSAGE("QAD_ResourceMgr::path : <"<<filename.latin1()<<"> : "<<filePath.latin1()) ;
  return filePath;
}

/*!
    Loads a pixmap from 'resname' resources 
    and indetified by 'id'
*/
QPixmap QAD_ResourceMgr::loadPixmap( const char* resname, 
                                     const QString& id ) const
{
  return QPixmap( path( id, resname, RES_PIXMAPS ) );
}

/*!
    Loads a doc page from 'resname' resources
    and indetified by 'id'
*/
bool QAD_ResourceMgr::loadDoc( const char* resname,
                               const QString& id ) const
{    
  QString docPath = path( id, resname, RES_DOCS );
  return true;
}

/************************************************************************
**  
**  Class QAD_ResourceMgr::ResourceSettings ( internal )
**  
*************************************************************************/

/*!
    Loads a resource 'file'. 
    Returns 'false' if 'file' can't be loaded( not found etc. ),
    'true' if loaded or reloaded OK. 
*/
bool QAD_ResourceMgr::ResourceSettings::load( const QString& file )
{   
#if QT_VERSION >= 0x030000 // VSR: workaround - crash on qt3.0.5 ==========
  static const int magic_length = 16;        // length of *.qm file header (qtranslator.cpp)
  static const uchar magic[magic_length] = { // magic number for the file
    0x3c, 0xb8, 0x64, 0x18, 0xca, 0xef, 0x9c, 0x95,
    0xcd, 0x21, 0x1c, 0xbf, 0x60, 0xa1, 0xbd, 0xdd };
  QFile f(file);
  if ( !f.exists() || f.size() < magic_length)
    return false;
  char buf[magic_length];
  if ( !f.open(IO_ReadOnly) )
    return false;
  bool bOk = ( f.readBlock(buf, magic_length) == magic_length );
  f.close();
  if (!bOk)
    return false;
  if ( memcmp( (const void *)buf, magic, magic_length ) )
    return false;
  if ( f.size() == magic_length)
    return true;
#endif // VSR =============================================================
  QTranslator* strTbl = new QTranslator( 0 );    
  try {
    if ( !strTbl->load( file, "" ) ) {
      delete strTbl;
      return false;
    }    
  }
  catch (...) {
    return false;
  }
  QAD_ASSERT_DEBUG_ONLY( qApp );
  qApp->installTranslator( strTbl );    
  return true;
}
