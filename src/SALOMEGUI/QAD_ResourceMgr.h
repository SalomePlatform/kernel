//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : QAD_ResourceMgr.cxx
//  Author : UI team
//  Module : SALOME
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

