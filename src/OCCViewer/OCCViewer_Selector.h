//  File      : OCCViewer_Selector.h
//  Created   : Wed Mar 20 10:56:53 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OCCViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef OCCViewer_SELECTOR_H
#define OCCViewer_SELECTOR_H

#include "QAD.h"

// QT Include
#include <qobject.h>

// Open CASCADE Include
#include <AIS_InteractiveObject.hxx>
#include <AIS_ListOfInteractive.hxx>

class QAD_EXPORT OCCViewer_Selector : public QObject
{
  Q_OBJECT	
		
 public:
  OCCViewer_Selector();
  virtual ~OCCViewer_Selector();

 public:
  void		enableSelection( bool );
  void		enableMultipleSelection( bool );

 signals:

  void		selSelectionDone( bool bAdded );
  void		selSelectionCancel( bool bAdded );
	
 protected:

  int		myNumSelected;
  bool		myEnableSelection;
  bool		myEnableMultipleSelection;
};

#endif

