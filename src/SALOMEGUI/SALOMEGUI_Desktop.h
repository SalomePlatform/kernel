//  File      : SALOMEGUI_Desktop.h
//  Created   : Thu Jun 14 12:02:21 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef SALOMEGUI_DESKTOP_H
#define SALOMEGUI_DESKTOP_H

#include "QAD_Desktop.h"
#include "QAD_Study.h"
class SALOMEGUI_Desktop : public QAD_Desktop
{
    Q_OBJECT

public:
    SALOMEGUI_Desktop( SALOME_NamingService* name_service );

    // returns the name of the component selected objects belong to (null if two or more components)
    QString getComponentFromSelection(); 

protected:
    void    updateDesktop( UpdateCommand  );

};

#endif

