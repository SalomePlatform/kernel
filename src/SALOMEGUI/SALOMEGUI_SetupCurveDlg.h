//  File      : SALOMEGUI_SetupCurveDlg.h
//  Created   : Wed Jun 27 16:38:42 2001
//  Author    : Vadim SANDLER
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
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

