//  File      : QAD_Splitter.h
//  Created   : Thu Jun 14 16:35:23 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_SPLITTER_H
#define QAD_SPLITTER_H

#include <qsplitterP.h>

class QSplitterP;
class QAD_Splitter : public QSplitterP
{
public:
    QAD_Splitter( QWidget *parent=0, const char *name=0 );
    QAD_Splitter( Orientation, QWidget *parent=0, const char *name=0 );
    ~QAD_Splitter();
};

#endif
