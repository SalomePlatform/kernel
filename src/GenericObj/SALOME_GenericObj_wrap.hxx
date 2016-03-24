// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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
// File      : SALOME_GenericObj_wrap.hxx
// Created   : Fri Dec  7 11:32:45 2012
// Author    : Edward AGAPOV (eap)


#ifndef __SALOME_GenericObj_wrap_HXX__
#define __SALOME_GenericObj_wrap_HXX__

namespace SALOME
{
  // call UnRegister on each element in a sequence of SALOME::GenericObj's
  template< class SEQ_OF_GENOBJ >
  void UnRegister( SEQ_OF_GENOBJ& seq )
  {
    for ( size_t i = 0; i < seq->length(); ++i )
      if ( !CORBA::is_nil( seq[i] ))
        seq[i]->UnRegister();
  }

  // This class is intended to free the user of objects, iherited from SALOME::GenericObj,
  // from a need to call UnRegister() after having finished using the object.
  // The behavior of this class is similar to that of var CORBA class and thus it
  // can replace var types of wrapped CORBA classes in some code like follows:
  //
  // namespace SALOMEDS
  // {
  //   typedef SALOME::GenericObj_wrap< SObject >       SObject_wrap;
  //   typedef SALOME::GenericObj_wrap< ChildIterator > ChildIterator_wrap;
  // }
  // ...
  // SALOMEDS::ChildIterator_wrap anIter = aStudy->NewChildIterator( mainSO );
  // for ( ; anIter->More(); anIter->Next() ) {
  //   SALOMEDS::SObject_wrap so = anIter->Value();
  // ...
  //
  // Note that the need to call UnRegister(), and thus to use GenericObj_wrap, depends on
  // implementation of a method returning a GenericObj servant. If the method creates a
  // servant, than UnRegister() must be called. If the method retrieves a servant from
  // some storage, then it's wrong to call UnRegister().
  //
  // A pleasant additional feature privided by this class is that one can assign
  // GenericObj_wrap< BASE_CLASS > to GenericObj_wrap< CHILD_CLASS > without
  // calling CHILD_CLASS::_narrow().
  //
  template< class GENOBJ >
  class GenericObj_wrap
  {
  public:
    typedef typename GENOBJ::_ptr_type TPtr;
    typedef typename GENOBJ::_var_type TVar;

    // CONSTRUCTION
    GenericObj_wrap()
    {
      _isOwn = false;
    }
    GenericObj_wrap( const TPtr& obj )
      : _obj( obj ), _isOwn( true )
    {}
    GenericObj_wrap( const TVar& obj )
      : _obj( obj ), _isOwn( true )
    {}
    GenericObj_wrap( const GenericObj_wrap& wrap )
      : _obj( wrap._obj ), _isOwn( wrap._getIsOwn() )
    {
      _register();
    }
    template< class BASE_GENOBJ >
    GenericObj_wrap( const GenericObj_wrap<BASE_GENOBJ>& wrap )
      : _obj( GENOBJ::_narrow( wrap.in() )), _isOwn( wrap._getIsOwn() )
    {
      _register();
    }
    // DESTRUCTION
    ~GenericObj_wrap()
    {
      _release();
    }
    // COPYING
    GenericObj_wrap& operator=( const TPtr& obj )
    {
      _release();
      _obj   = obj;
      _isOwn = true;
      return *this;
    }
    GenericObj_wrap& operator=( const TVar& obj )
    {
      _release();
      _obj   = obj;
      _isOwn = true;
      return *this;
    }
    GenericObj_wrap& operator=( const GenericObj_wrap& wrap )
    {
      _release();
      _obj   = wrap._obj;//wrap.in();
      _isOwn = wrap._getIsOwn();
      _register();
      return *this;
    }
    template< class BASE_GENOBJ >
    GenericObj_wrap& operator=( const GenericObj_wrap<BASE_GENOBJ>& wrap )
    {
      _release();
      _obj   = GENOBJ::_narrow( wrap.in() );
      _isOwn = wrap._getIsOwn();
      _register();
      return *this;
    }
    // ACCESS
    operator TPtr()
    {
      return _obj.in();
    }
    operator TVar()
    {
      return _obj;
    }
    TVar operator->()
    {
      return _obj;
    }
    TPtr in() const
    {
      return _obj;
    }
    TPtr& inout()
    {
      _release(); // returned reference to _obj will be initialized with another object
      _isOwn = true;
      return _obj.inout();
    }
    TPtr _retn()
    {
      _isOwn = false;
      return _obj._retn();
    }

    bool _getIsOwn() const { return  _isOwn; }

  private:
    void _register()
    {
      if ( _isOwn && !CORBA::is_nil( _obj ))
        _obj->Register();
    }
    void _release()
    {
      if ( _isOwn && !CORBA::is_nil( _obj ))
        _obj->UnRegister();
      _isOwn = false;
    }
    TVar _obj;
    bool _isOwn;
  };
} // namespace SALOME

#endif
