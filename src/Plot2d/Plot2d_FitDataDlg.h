//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_FitDataDlg.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#if !defined(Plot2d_FitDataDlg_H)
#define Plot2d_FitDataDlg_H

#include <qdialog.h>

#include <qspinbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qpushbutton.h>

//================================================================
// Class        : Plot2d_FitDataDlg
// Description  : Dialog to fit view for given data area
//================================================================
class Plot2d_FitDataDlg : public QDialog
{
  Q_OBJECT
public:
// constuctor
  Plot2d_FitDataDlg(QWidget* parent);

// sets range
  void setRange(const double xMin, 
                const double xMax,
                const double yMin,
                const double yMax);
// gets range, returns mode (see getMode())
  int getRange(double& xMin, 
               double& xMax,
               double& yMin,
               double& yMax);
// gets mode : 0 - Fit all; 1 - Fit horizontal, 2 - Fit vertical
  int getMode();

protected slots:
// called when range mode changed
  void onModeChanged(int);

private:
  QButtonGroup*           myRangeGrp;
  QRadioButton*           myModeAllRB;
  QRadioButton*           myModeHorRB;
  QRadioButton*           myModeVerRB;
  QLineEdit*              myXMinEdit;
  QLineEdit*              myYMinEdit;
  QLineEdit*              myXMaxEdit;
  QLineEdit*              myYMaxEdit;
  QPushButton*            myOkBtn;
  QPushButton*            myCancelBtn;
};

#endif // !defined(Plot2d_FitDataDlg_H)
