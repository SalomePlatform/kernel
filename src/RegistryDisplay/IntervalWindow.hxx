//  SALOME RegistryDisplay : GUI for Registry server implementation
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : IntervalWindow.hxx
//  Author : Oksana TCHEBANOVA
//  Module : SALOME

#ifndef IntervalWindow_HeaderFile
#define IntervalWindow_HeaderFile

# include <qwidget.h>
# include <qdialog.h>
# include <qpushbutton.h>
# include <qspinbox.h>


class IntervalWindow : public QDialog
{
public:
  IntervalWindow( QWidget* parent = 0 );
  ~IntervalWindow();
  
  QPushButton* Ok();
  QPushButton* Cancel();

  int getValue();
  void setValue( int );

private:
  QSpinBox* mySpinBox;
  QPushButton* myButtonOk;
  QPushButton* myButtonCancel;
};

#endif
