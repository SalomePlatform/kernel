//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_Curve.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef Plot2d_Curve_h
#define Plot2d_Curve_h

#include <qstring.h>
#include <qcolor.h>
#include <qmemarray.h>
#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif
#include "SALOME_InteractiveObject.hxx"

typedef QMemArray<double> DoubleArray;

class Plot2d_Curve 
{
public:
  enum MarkerType { None,      Circle,    Rectangle, Diamond,
		    DTriangle, UTriangle, LTriangle, RTriangle,
		    Cross, XCross };
  enum LineType { NoPen, Solid, Dash, Dot, DashDot, DashDotDot };

  Plot2d_Curve();
  ~Plot2d_Curve();
  Plot2d_Curve( const Plot2d_Curve& curve );
  Plot2d_Curve& operator= ( const Plot2d_Curve& curve );

  void        setHorTitle( const QString& title );
  QString     getHorTitle() const;
  void        setVerTitle( const QString& title );
  QString     getVerTitle() const;
  void        setHorUnits( const QString& units );
  QString     getHorUnits() const;
  void        setVerUnits( const QString& units );
  QString     getVerUnits() const;
  void        setData( const double* hData, const double* vData, long size );
  double*     horData() const;
  double*     verData() const;
  long        nbPoints() const;
  bool        isEmpty() const;

  void        setAutoAssign( bool on );
  bool        isAutoAssign() const;
  void        setColor( const QColor color );
  QColor      getColor() const;
  void        setMarker( MarkerType marker );
  MarkerType  getMarker() const;
  void        setLine( LineType line, const int lineWidth = 0 );
  LineType    getLine() const;
  int         getLineWidth() const;

  virtual bool                             hasIO() { return !myIO.IsNull(); }
  virtual Handle(SALOME_InteractiveObject) getIO() { return myIO; }
  virtual void                             setIO( const Handle(SALOME_InteractiveObject)& io ) { myIO = io; }

  virtual bool                             hasTableIO() { return !myTableIO.IsNull(); }
  virtual Handle(SALOME_InteractiveObject) getTableIO() { return myTableIO; }
  virtual void                             setTableIO( const Handle(SALOME_InteractiveObject)& io ) { myTableIO = io; }
  
private:
  Handle(SALOME_InteractiveObject) myIO;
  Handle(SALOME_InteractiveObject) myTableIO;

  bool        myAutoAssign;
  QString     myHorTitle;
  QString     myVerTitle;
  QString     myHorUnits;
  QString     myVerUnits;
  QColor      myColor;
  MarkerType  myMarker;
  LineType    myLine;
  int         myLineWidth;
  DoubleArray myHorData;
  DoubleArray myVerData;
};

#endif // Plot2d_Curve_h


