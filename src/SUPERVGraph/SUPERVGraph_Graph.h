//  File      : SUPERVGraph_Graph.h
//  Created   : 17 / 10 / 2001
//  Author    : Francis KLOSS
//  Project   : SALOME
//  Module    : SUPERVGraph
//  Copyright : Open CASCADE

#ifndef SUPERVGraph_Graph_HXX
#define SUPERVGraph_Graph_HXX

#include "QAD.h"
#include "QAD_Popup.h"

#include <qscrollview.h>
#include <qpopupmenu.h>

class QAD_EXPORT SUPERVGraph_Graph: public QScrollView, public QAD_PopupClientServer
{
  Q_OBJECT

public:
  SUPERVGraph_Graph(QWidget* parent);
  ~SUPERVGraph_Graph();

  /* popup management */
  void onCreatePopup();

  void sync();
  void contentsMousePressEvent(QMouseEvent* e);
  void contentsMouseReleaseEvent(QMouseEvent* e);
  void contentsMouseMoveEvent(QMouseEvent* e);
  
  QPoint getPoint();
  
public slots:
  void Draw(int x, int y);
  
private slots:
  void addPoint();
  void deleteLink();
  void showInformation();
  void changeInformation();

protected slots:
  void onChangeBackgroundColor();

protected:
  bool eventFilter(QObject* object, QEvent* event);
  
private:
//    SUPERVGraph_Point* findPoint(int x, int y);
  float distance(int xp, int yp, int xl, int yl, int xn, int yn);
  
//    SUPERVGraph_Main* main;
  QPoint          where;
  QPopupMenu*     popup;
  int             pci;
  int             pan;
  int             pap;
  int             pdl;
  int             pif;
};

#endif
