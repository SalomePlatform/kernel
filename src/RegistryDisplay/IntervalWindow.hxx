// File:	IntervalWindow.hxx
// Created:	Thu Apr  3 10:31:10 2003
// Author:	Oksana TCHEBANOVA
//		<ota@parcex.nnov.matra-dtv.fr>


#ifndef IntervalWindow_HeaderFile
#define IntervalWindow_HeaderFile

# include <qwidget.h>
# include <qdialog.h>
# include <qpushbutton.h>
# include <qspinbox.h>

using namespace std;

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
