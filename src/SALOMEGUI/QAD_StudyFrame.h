//  File      : QAD_StudyFrame.h
//  Created   : Wed Mar 20 11:10:03 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef QAD_StudyFrame_H
#define QAD_StudyFrame_H

#include "QAD.h" 
#include "QAD_Splitter.h" 
#include "QAD_PyInterp.h"

// QT Includes
#include <qwidget.h>
#include <qmainwindow.h>

class QAD_RightFrame;
class QAD_LeftFrame;
class QAD_Splitter;
class QAD_Study;

enum ViewType {
  VIEW_OCC,
  VIEW_VTK,
  VIEW_GRAPHSUPERV,
  VIEW_PLOT2D,
  VIEW_TYPE_MAX  // must always be the last one; insert new values before it
};

class QAD_EXPORT QAD_StudyFrame: public QMainWindow
{
  Q_OBJECT

 public:
  QAD_StudyFrame(QAD_Study* study,
		 QWidget* parent, const QString& title, 
		 QAD_PyInterp* interp, ViewType typeView);
  QAD_StudyFrame(QAD_Study*,
		 QWidget* parent = 0);
  virtual ~QAD_StudyFrame();

  QAD_Study*          getStudy() { return myStudy; }
  QAD_RightFrame*     getRightFrame() const;
  QAD_LeftFrame*      getLeftFrame() const;

  ViewType            getTypeView() const; 

  QAD_PyInterp*       get_PyInterp(void);

  void		      setTitle( const QString& aTitle);
  const QString&      title() const;

  void		      setEntry( const QString& aTitle);
  const QString&      entry() const;

  void                setVisible( bool isVisible = true );
  void                closeEvent(QCloseEvent* e);

  void                compressLeft();
  void                compressRight();
  void                unCompressLeft();
  void                unCompressRight();

 signals:
  void                sfStudyFrameActivated( QAD_StudyFrame* );
  void                sfStudyFrameClosing( QAD_StudyFrame*);
  void                sfBeforeStudyFrameClosing( QAD_StudyFrame*);

 public slots:
  void      	      onStudyFrameActivated ( QWidget* );
	

 private:
  QString	      myTitle;	/* title */
  QString	      myEntry;	/* entry */

  ViewType            myTypeView;
  QAD_LeftFrame*      myLeftFrm;
  QAD_RightFrame*     myRightFrm;
  QAD_Splitter*       s1; 
  QAD_PyInterp*       _interp;
  QAD_Study*          myStudy;
};

#endif
