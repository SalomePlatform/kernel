//  File      : SALOMEGUI_TrihedronSizeDlg.h
//  Created   : Fri Mar 22 16:33:18 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef SALOMEGUI_TRIHEDRON_SIZE_DLG_H
#define SALOMEGUI_TRIHEDRON_SIZE_DLG_H

#include <qdialog.h>

class QSpinBox;

class SALOMEGUI_TrihedronSizeDlg : public QDialog
{ 
  Q_OBJECT

public:
    SALOMEGUI_TrihedronSizeDlg( QWidget* parent = 0 );
    ~SALOMEGUI_TrihedronSizeDlg();

    void setTrihedronSize(const int size);
    int  getTrihedronSize();

protected:
    QSpinBox* SpinBox;
};

#endif // SALOMEGUI_TRIHEDRON_SIZE_DLG_H
