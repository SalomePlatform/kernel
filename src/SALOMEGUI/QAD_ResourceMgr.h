//  File      : QAD_ResourceMgr.cxx
//  Created   : UI team, 22.10.00
//  Descr     : ResourceMgr QAD-based application
//  Author    : UI team
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$


#ifndef QAD_RESOURCEMGR_H
#define QAD_RESOURCEMGR_H

#include <qdict.h>
#include <qpixmap.h>
#include <qcstring.h>
#include <qasciidict.h>
#include <qtranslator.h>

#include "QAD.h"
class QAD_EXPORT QAD_ResourceMgr : public QObject
{
    Q_OBJECT

    typedef QDict<QString>  StringDict;
    class ResourceSettings 
    {
        StringDict      myConfig;
     public:
         ResourceSettings() : myConfig( 10, false ) 
            { myConfig.setAutoDelete( true ); }
         ~ResourceSettings() 
            { myConfig.clear(); }

        StringDict& config() { return myConfig; }
        bool        load( const QString& filename );        
    };

public:
    QAD_ResourceMgr();
    ~QAD_ResourceMgr();

public:
    bool                    loadResources( const char* resname, QString& msg );    
    QPixmap                 loadPixmap( const char* resname,
                                        const QString& id ) const;  
    bool                    loadDoc( const char* resname, 
                                     const QString& id ) const;
    QString                 language( const char* prefix ) const;
    QString                 resources( const char* prefix ) const;

    QString                 findFile(const QString& filename, const char* prefix ) const;
    QString                 getFile( const QString& filename, const char* prefix ) const;

    bool                    removeResources( const char* resname );

private:
    QString                 path( const QString& filename, 
				  const char* resname, 
                                  const char* key ) const;
    QString                 collectDirs( const QString& prefix ) const;

private:
    QAsciiDict<ResourceSettings> myRes;     /* prefix <-> resources  */         
};

#endif

