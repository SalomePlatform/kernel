//  SALOME SUPERVGraph : build Supervisor viewer into desktop
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
//  File   : SUPERVGraph_Graph.cxx
//  Author : Francis KLOSS
//  Module : SALOME

#include "SUPERVGraph_Graph.h"

#include "QAD_Settings.h"
#include "QAD_Config.h"

#include <qcolordialog.h>
#include <qpopupmenu.h>
using namespace std;


SUPERVGraph_Graph::SUPERVGraph_Graph(QWidget* parent) : 
  QScrollView(parent),
  where(QPoint(0, 0))
{
  installEventFilter(viewport());
  //  viewport()->setBackgroundColor(QColor(165, 255, 176));

  connect(this, SIGNAL(contentsMoving(int, int)), this, SLOT(Draw(int, int)));
  
  popup = new QPopupMenu(viewport());
  //      if (main->isModify()) {
  //          pan = popup->insertItem("Add Node",            main, SLOT(addNode()));
  //          pap = popup->insertItem("Add Point",           this, SLOT(addPoint()));
  //          pdl = popup->insertItem("Delete Link",         this, SLOT(deleteLink()));
  //          pif = popup->insertItem("Insert File",         main, SLOT(insertFile()));
  //          pci = popup->insertItem("Change Informations", this, SLOT(changeInformation()));
  //                popup->insertSeparator();
  //      };
  //    popup->insertItem("Add in Study",        main, SLOT(addStudy()));
  //    popup->insertItem("Show Informations",   this, SLOT(showInformation()));
  //    popup->insertSeparator();
  //    popup->insertItem("Copy (Read Only)",    main, SLOT(copyReading()));
  //    popup->insertItem("Copy (Modifiying)",   main, SLOT(copyModifying()));
}

void SUPERVGraph_Graph::sync() {
  //   bool editing = main->getDataflow()->IsEditing();
  //      if (main->isModify()) {
  //          popup->setItemEnabled(pci, editing);
  //          popup->setItemEnabled(pan, editing);
  //          popup->setItemEnabled(pap, editing);
  //          popup->setItemEnabled(pdl, editing);
  //          popup->setItemEnabled(pif, editing);
  //      };
  
  //      SUPERVGraph_Node* ihmNode;
  //      QObjectList* ihmList = queryList("SUPERVGraph_Node");
  //      SUPERV_Nodes nodes = main->getDataflow()->Nodes();
  //      int n = nodes->length();

  //      for (int i=0; i<n; i++) {
  //          ihmNode = (SUPERVGraph_Node *)child(nodes[i]->Name(), "SUPERVGraph_Node");
  //          if (ihmNode == NULL) {
  //              ihmNode = new SUPERVGraph_Node(viewport(), main, nodes[i]);
  //              addChild(ihmNode, nodes[i]->X(), nodes[i]->Y());
  //          } else {
  //              moveChild(ihmNode, nodes[i]->X(), nodes[i]->Y());
  //  	    ihmList->removeRef(ihmNode);
  //          };
  //          ihmNode->sync();
  //      };

  //      QObjectListIt i(*ihmList);
  //      while ((ihmNode=(SUPERVGraph_Node*)i.current()) != 0) {
  //          ++i;
  //          ihmNode->close();
  //      };
  //      delete ihmList;

  //      SUPERVGraph_PortIn* pi;
  //      ihmList = queryList("SUPERVGraph_PortIn");
  //      i = *ihmList;
  //      while ((pi=(SUPERVGraph_PortIn*)i.current()) != 0) {
  //          ++i;
  //          pi->sync(true);
  //      };
  //      delete ihmList;

  //      SUPERVGraph_PortOut* po;
  //      ihmList = queryList("SUPERVGraph_PortOut");
  //      i = *ihmList;
  //      while ((po=(SUPERVGraph_PortOut*)i.current()) != 0) {
  //          ++i;
  //          po->sync();
  //      };
  //      delete ihmList;

  //      Draw(0, 0);
}

SUPERVGraph_Graph::~SUPERVGraph_Graph() {
}

bool SUPERVGraph_Graph::eventFilter(QObject* object, QEvent* event) {
  if ((event->type() == QEvent::Move ) ||
      (event->type() == QEvent::Paint) ||
      (event->type() == QEvent::Resize) ) {
    Draw(0, 0);
  };
  return(QScrollView::eventFilter(object, event));
}

void SUPERVGraph_Graph::Draw(int x, int y) {
  QPainter p(viewport());
  p.eraseRect(viewport()->rect());
  p.setPen(QPen(Qt::black, 2));
  
  //   SUPERVGraph_Point* c;
  //    SUPERVGraph_Point* n;
  //    QObjectList* points = queryList("SUPERVGraph_Point");
  //    QObjectListIt i(*points);
  //    while ((c=(SUPERVGraph_Point*)i.current()) != 0) {
  //      ++i;
  //          n=c->getNext();
  //          if (n!=c) {
  //              p.drawLine(contentsToViewport(QPoint(c->getX()+POINT_SIZE_HALF, c->getY()+POINT_SIZE_HALF)),
  //                         contentsToViewport(QPoint(n->getX()+POINT_SIZE_HALF, n->getY()+POINT_SIZE_HALF)) );
  //          };
  //      };
  //      delete points;
}

void SUPERVGraph_Graph::contentsMousePressEvent(QMouseEvent* e) {
  where = viewportToContents(viewport()->mapFromGlobal(e->globalPos()));
  //   main->showPopup(popup, e);
}

void SUPERVGraph_Graph::contentsMouseReleaseEvent(QMouseEvent* e) {
  //    main->sketchPoint(NULL);

 if ( e->button() == Qt::RightButton ) {
   QPopupMenu* popup = createPopup();
   if ( popup ) {
     popup->exec( QCursor::pos() );
     destroyPopup();
   }
   return;
 }  

}

void SUPERVGraph_Graph::contentsMouseMoveEvent(QMouseEvent* e) {
  //   main->sketchMove();
}

float SUPERVGraph_Graph::distance(int xp, int yp, int xl, int yl, int xn, int yn) {
  float r, s;
  float a, b, c, d;

  float px = (float)(xp);
  float py = (float)(yp);
  float lx = (float)(xl);
  float ly = (float)(yl);
  float nx = (float)(xn);
  float ny = (float)(yn);

  r = px - lx;
  r = r * r;
  s = py - ly;
  s = s * s;
  a = r + s;

  r = px - nx;
  r = r * r;
  s = py - ny;
  s = s * s;
  b = r + s;

  r = nx - lx;
  r = r * r;
  s = ny - ly;
  s = s * s;
  c = r + s;

  if (c<0.001) {
    return(a);
  } else {
    r = a + c - b;
    r = r * r;
    d = r / ( 4 * c);
    return(a - d);
  };
}

QPoint SUPERVGraph_Graph::getPoint() {
  return(where);
}

//  SUPERVGraph_Point* SUPERVGraph_Graph::findPoint(int x, int y) {
//      float            m;
//      float            d = SCROLL_MAX;
//      SUPERVGraph_Point* t = NULL;
//      SUPERVGraph_Point* c;
//      SUPERVGraph_Point* n;
//      QObjectList* points = queryList("SUPERVGraph_Point");
//      QObjectListIt i(*points);

//      while ((c=(SUPERVGraph_Point*)i.current()) != 0) {
//          ++i;
//          n=c->getNext();
//          if (n != c) {
//              m = distance(x, y, c->getX(), c->getY(), n->getX(), n->getY());
//              if (m < d) {
//                  d = m;
//                  t = c;
//              };
//          };
//      };
//      delete points;
//      return(t);
//  }

void SUPERVGraph_Graph::addPoint() {
  //    int              x = where.x();
  //      int              y = where.y();
  //      SUPERVGraph_Point* p = findPoint(x, y);
  //      if (p == NULL) {
  //          QMessageBox::warning(0, "Supervision Error", "No Link to Add a Point");
  //      } else {
  //          p->addPoint(x, y, true);
  //      };
}

void SUPERVGraph_Graph::deleteLink() {
  //   SUPERVGraph_Point* p = findPoint(where.x(), where.y());
  //      if (p == NULL) {
  //          QMessageBox::warning(0, "Supervision Error", "No Link to Delete");
  //      } else {
  //          SUPERV_Link l = p->getLink();
  //          if (l == NULL) {
  //              QMessageBox::warning(0, "Supervision Error", "No Link to Delete");
  //          } else {
  //              l->Destroy();
  //              main->sync();
  //          };
  //      };
}

void SUPERVGraph_Graph::showInformation() {
  //   if (Supervision.information(Engines::Node::_narrow(main->getDataflow()), false)) {
  //          main->sync();
  //      };
}

void SUPERVGraph_Graph::changeInformation() {
  //    if (Supervision.information(Engines::Node::_narrow(main->getDataflow()), true)) {
  //          main->sync();
  //      };
}

/*!
    Creates the popup 
*/
void SUPERVGraph_Graph::onCreatePopup() 
{
  if ( myPopup ) {			
//    if (myPopup->count()<1) {
    if ( myPopup->count() > 0 )
      myIDs.append ( myPopup->insertSeparator() );	
    int id;
    myIDs.append ( id = myPopup->insertItem (tr ("MEN_VP3D_CHANGEBGR")) );	
    QAD_ASSERT ( myPopup->connectItem ( id, this, SLOT(onChangeBackgroundColor())) );
//    }
  }
}

void SUPERVGraph_Graph::onChangeBackgroundColor()
{
  QColor previousColor = viewport()->backgroundColor();
  QColor selColor = QColorDialog::getColor ( previousColor, NULL );	
  if ( selColor.isValid() ) 
    viewport()->setBackgroundColor( selColor );
  
  QAD_CONFIG->addSetting( "SUPERVGraph:BackgroundColorRed",   selColor.red() );
  QAD_CONFIG->addSetting( "SUPERVGraph:BackgroundColorGreen", selColor.green() );
  QAD_CONFIG->addSetting( "SUPERVGraph:BackgroundColorBlue",  selColor.blue() );
}
