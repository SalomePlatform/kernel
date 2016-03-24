// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : SALOMEDSImpl_AttributeTable.hxx
//  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//
#ifndef _SALOMEDSImpl_AttributeTable_HeaderFile
#define _SALOMEDSImpl_AttributeTable_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AttributeTable
{
public:
  //! Sort order
  typedef enum { 
    AscendingOrder,  //!< The items are sorted ascending
    DescendingOrder, //!< The items are sorted descending
  } SortOrder;

  //! Sort policy (specifies how empty cells are taken into account when sorting)
  typedef enum {
    EmptyLowest,     //!< Empty cells are considered as lowest values
    EmptyHighest,    //!< Empty cells are considered as highest values
    EmptyFirst,      //!< Empty cells are always first
    EmptyLast,       //!< Empty cells are always last
    EmptyIgnore,     //!< Empty cells are ignored (stay at initial positions)
  } SortPolicy;
};

template <class TTable> class TableSorter
{
  TTable*                                 myTable;
  SALOMEDSImpl_AttributeTable::SortOrder  mySortOrder;
  SALOMEDSImpl_AttributeTable::SortPolicy mySortPolicy;
  int                                     myIndex;
  bool                                    myIsRow;

public:
  TableSorter( TTable*                                 table,
	       SALOMEDSImpl_AttributeTable::SortOrder  so,
	       SALOMEDSImpl_AttributeTable::SortPolicy sp,
	       int                                     index,
	       bool                                    sortRow)
    : myTable( table ), mySortOrder( so ), mySortPolicy( sp ), 
      myIndex( index ), myIsRow( sortRow ) {}

  bool operator() ( int idx1, int idx2 ) 
  {
    bool hasValue1 = myIsRow ? myTable->HasValue( myIndex, idx1 ) : myTable->HasValue( idx1, myIndex );
    bool hasValue2 = myIsRow ? myTable->HasValue( myIndex, idx2 ) : myTable->HasValue( idx2, myIndex );
    if ( !hasValue1 && !hasValue2 ) {
      return false;
    }
    else if ( !hasValue1 || !hasValue2 ) {
      switch ( mySortPolicy ) {
      case SALOMEDSImpl_AttributeTable::EmptyLowest:
	return (!hasValue1) ? 
	  (mySortOrder == SALOMEDSImpl_AttributeTable::AscendingOrder) : 
	  (mySortOrder == SALOMEDSImpl_AttributeTable::DescendingOrder);
      case SALOMEDSImpl_AttributeTable::EmptyHighest:
	return (!hasValue1) ? 
	  (mySortOrder != SALOMEDSImpl_AttributeTable::AscendingOrder) : 
	  (mySortOrder != SALOMEDSImpl_AttributeTable::DescendingOrder);
      case SALOMEDSImpl_AttributeTable::EmptyFirst:
	return (!hasValue1);
      case SALOMEDSImpl_AttributeTable::EmptyLast:
	return hasValue1;
      case SALOMEDSImpl_AttributeTable::EmptyIgnore:
      default:
	// should not go here
	return false;
      }
    }
    else {
      if ( myIsRow ) {
	return mySortOrder == SALOMEDSImpl_AttributeTable::AscendingOrder ? 
	  myTable->GetValue( myIndex, idx1 ) < myTable->GetValue( myIndex, idx2 ) :
	  myTable->GetValue( myIndex, idx2 ) < myTable->GetValue( myIndex, idx1 );
      }
      else {
	return mySortOrder == SALOMEDSImpl_AttributeTable::AscendingOrder ? 
	  myTable->GetValue( idx1, myIndex ) < myTable->GetValue( idx2, myIndex ) :
	  myTable->GetValue( idx2, myIndex ) < myTable->GetValue( idx1, myIndex );
      }
    }
  }
};

#endif // _SALOMEDSImpl_AttributeTable_HeaderFile
