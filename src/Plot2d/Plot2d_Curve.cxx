//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_Curve.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "Plot2d_Curve.h"
#include "utilities.h"
using namespace std;

/*!
  Constructor
*/
Plot2d_Curve::Plot2d_Curve()
     : myHorTitle( "" ), myVerTitle( "" ), 
       myHorUnits( "" ), myVerUnits( "" ), 
       myAutoAssign( true ), myColor( 0,0,0 ), myMarker( Circle ), myLine( Solid ), myLineWidth( 0 )
{
}
/*!
  Destructor
*/
Plot2d_Curve::~Plot2d_Curve()
{
//  MESSAGE("Plot2d_Curve::~Plot2d_Curve");
}
/*!
  Copy constructor. Makes deep copy of data.
*/
Plot2d_Curve::Plot2d_Curve( const Plot2d_Curve& curve )
{
  myAutoAssign = curve.isAutoAssign();
  myHorTitle   = curve.getHorTitle();
  myVerTitle   = curve.getVerTitle();
  myHorUnits   = curve.getHorUnits();
  myVerUnits   = curve.getVerUnits();
  myColor      = curve.getColor();
  myMarker     = curve.getMarker();
  myLine       = curve.getLine();
  myLineWidth  = curve.getLineWidth();
  myHorData.duplicate( curve.horData(), curve.nbPoints() );
  myVerData.duplicate( curve.verData(), curve.nbPoints() );
}
/*!
  operator=. Makes deep copy of data.
*/
Plot2d_Curve& Plot2d_Curve::operator=( const Plot2d_Curve& curve )
{
  myAutoAssign = curve.isAutoAssign();
  myHorTitle   = curve.getHorTitle();
  myVerTitle   = curve.getVerTitle();
  myHorUnits   = curve.getHorUnits();
  myVerUnits   = curve.getVerUnits();
  myColor      = curve.getColor();
  myMarker     = curve.getMarker();
  myLine       = curve.getLine();
  myLineWidth  = curve.getLineWidth();
  myHorData.duplicate( curve.horData(), curve.nbPoints() );
  myVerData.duplicate( curve.verData(), curve.nbPoints() );
  return *this;
}
/*!
  Sets curve's horizontal title
*/
void Plot2d_Curve::setHorTitle( const QString& title )
{
  myHorTitle = title;
}
/*!
  Gets curve's horizontal title
*/
QString Plot2d_Curve::getHorTitle() const
{
  return myHorTitle;
}
/*!
  Sets curve's vertical title
*/
void Plot2d_Curve::setVerTitle( const QString& title )
{
  myVerTitle = title;
}
/*!
  Gets curve's vertical title
*/
QString Plot2d_Curve::getVerTitle() const
{
  return myVerTitle;
}
/*!
  Sets curve's horizontal units
*/
void Plot2d_Curve::setHorUnits( const QString& units )
{
  myHorUnits = units;
}
/*!
  Gets curve's horizontal units
*/
QString Plot2d_Curve::getHorUnits() const
{
  return myHorUnits;
}
/*!
  Sets curve's vertical units
*/
void Plot2d_Curve::setVerUnits( const QString& units )
{
  myVerUnits = units;
}
/*!
  Gets curve's vertical units
*/
QString Plot2d_Curve::getVerUnits() const
{
  return myVerUnits;
}
/*!
  Sets curve's data. Makes shallow copy of data.
*/
void Plot2d_Curve::setData( const double* hData, const double* vData, long size )
{
  myHorData.assign( hData, size );
  myVerData.assign( vData, size );
}
/*!
  Gets curve's data : abscissas of points
*/
double* Plot2d_Curve::horData() const
{
  return myHorData.data();
}
/*!
  Gets curve's data : ordinates of points
*/
double* Plot2d_Curve::verData() const
{
  return myVerData.data();
}
/*!
  Gets curve's data : number of points
*/
long Plot2d_Curve::nbPoints() const
{
  return myHorData.size();
}
/*!
  Returns true if curve has no data
*/
bool Plot2d_Curve::isEmpty() const
{
  return ( myHorData.isEmpty() || myVerData.isEmpty() );
}
/*!
  Sets curve's AutoAssign flag - in this case attributes will be set automatically
*/
void Plot2d_Curve::setAutoAssign( bool on )
{
  myAutoAssign = on;
}
/*!
  Gets curve's AutoAssign flag state
*/
bool Plot2d_Curve::isAutoAssign() const
{
  return myAutoAssign;
}
/*!
  Sets curve's color ( and resets AutoAssign flag )
*/
void Plot2d_Curve::setColor( const QColor color )
{
  myColor = color;
  myAutoAssign = false;
}
/*!
  Gets curve's color
*/
QColor Plot2d_Curve::getColor() const
{
  return myColor;
}
/*!
  Sets curve's marker ( and resets AutoAssign flag )
*/
void Plot2d_Curve::setMarker( MarkerType marker )
{
  myMarker = marker;
  myAutoAssign = false;
}
/*!
  Gets curve's marker
*/
Plot2d_Curve::MarkerType Plot2d_Curve::getMarker() const
{
  return myMarker;
}
/*!
  Sets curve's line type and width ( and resets AutoAssign flag )
  NOTE : A line width of 0 will produce a 1 pixel wide line using a fast algorithm for diagonals. 
         A line width of 1 will also produce a 1 pixel wide line, but uses a slower more accurate algorithm for diagonals. 
         For horizontal and vertical lines a line width of 0 is the same as a line width of 1.
*/
void Plot2d_Curve::setLine( LineType line, const int lineWidth )
{
  myLine = line;
  myLineWidth = lineWidth;
  if ( myLineWidth < 0 ) myLineWidth = 0;
  myAutoAssign = false;
}
/*!
  Gets curve's line type
*/
Plot2d_Curve::LineType Plot2d_Curve::getLine() const
{
  return myLine;
}
/*!
  Gets curve's line width
*/
int Plot2d_Curve::getLineWidth() const
{
  return myLineWidth;
}
