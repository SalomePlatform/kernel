//  File      : OCCViewer_AISSelector.h
//  Created   : Wed Mar 20 10:58:22 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OCCViewer
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef OCCViewer_AISSELECTOR_H
#define OCCViewer_AISSELECTOR_H

#include "QAD.h"
#include "OCCViewer_Selector.h"

// Open CASCADE Includes
#include <Quantity_NameOfColor.hxx>
#include <AIS_InteractiveContext.hxx>

class QAD_EXPORT OCCViewer_AISSelector : public OCCViewer_Selector
{
  Q_OBJECT

  void	initialize();
  void	cleanup();

public:
  OCCViewer_AISSelector( const Handle (AIS_InteractiveContext)& );
  ~OCCViewer_AISSelector();

public:
  bool	moveTo ( int, int, const Handle (V3d_View)& );
  bool	select ( int, int, int, int, const Handle (V3d_View)& );
  bool	shiftSelect ( int, int, int, int, const Handle (V3d_View)& );
  bool	select ();
  bool	shiftSelect ();

  void	setContext ( const Handle (AIS_InteractiveContext)& );
  void	setHilightColor ( Quantity_NameOfColor color );
  void	setSelectColor ( Quantity_NameOfColor color );

protected:
  bool	checkSelection ( AIS_StatusOfPick status, bool hadSelection, bool addTo );

protected:
  Handle (AIS_InteractiveContext) myAISContext;	  // graphic context
  Quantity_NameOfColor		  myHilightColor; // color for hilight object
  Quantity_NameOfColor		  mySelectColor; // color for selected object

};

#endif

