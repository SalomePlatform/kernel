//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_SetupCurveDlg.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_SetupCurveDlg_H
#define SALOMEGUI_SetupCurveDlg_H

#include <qdialog.h>

//=================================================================================
// class    : SALOMEGUI_SetupCurveDlg
// purpose  : Dialog box for setup Plot2d curve
//=================================================================================

class QPushButton;
class QComboBox;
class QSpinBox;
class QToolButton;

class SALOMEGUI_SetupCurveDlg : public QDialog
{ 
  Q_OBJECT

public:
  SALOMEGUI_SetupCurveDlg( QWidget* parent = 0 );
  ~SALOMEGUI_SetupCurveDlg();

public:
  void   setLine( const int line, const int width );
  int    getLine() const;
  int    getLineWidth() const;
  void   setMarker( const int marker );
  int    getMarker() const ;
  void   setColor( const QColor& color );
  QColor getColor() const;
  
protected slots:
  void   onColorChanged();

private:
  QPushButton* myOkBtn;
  QPushButton* myCancelBtn;
  QComboBox*   myLineCombo;
  QSpinBox*    myLineSpin;
  QComboBox*   myMarkerCombo;
  QToolButton* myColorBtn;
};

#endif // SALOMEGUI_SetupCurveDlg_H

