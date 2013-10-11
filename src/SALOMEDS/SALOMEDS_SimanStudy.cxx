// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

//  File   : SALOMEDS_SimanStudy.cxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//
#include "utilities.h" 

#include "SALOMEDS_SimanStudy.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_Study.hxx"

#include "SALOMEDSImpl_Study.hxx"

#include "SALOMEDS_SimanStudy_i.hxx"

#include "Basics_Utils.hxx"

#include "Utils_ORB_INIT.hxx" 
#include "Utils_SINGLETON.hxx" 

/*
SALOMEDS_SimanStudy::SALOMEDS_SimanStudy(SALOMEDSImpl_SimanStudy* theStudy)
{
  _isLocal = true;
  _local_impl = theStudy;
  _corba_impl = SALOMEDS::SimanStudy::_nil();
  init_orb();
}*/

SALOMEDS_SimanStudy::SALOMEDS_SimanStudy(SALOMEDS::SimanStudy_ptr theStudy)
{
  /*
#ifdef WIN32
  long pid =  (long)_getpid();
#else
  long pid =  (long)getpid();
#endif  

  long addr = theStudy->GetLocalImpl(Kernel_Utils::GetHostname().c_str(), pid, _isLocal);
  if(_isLocal) {
    _local_impl = reinterpret_cast<SALOMEDSImpl_SimanStudy*>(addr);
    _corba_impl = SALOMEDS::SimanStudy::_duplicate(theStudy);
  }
  else {
    _local_impl = NULL;
    _corba_impl = SALOMEDS::SimanStudy::_duplicate(theStudy);
  }
  */
  _corba_impl = SALOMEDS::SimanStudy::_duplicate(theStudy);

  init_orb();
}

SALOMEDS_SimanStudy::~SALOMEDS_SimanStudy()
{
}

void SALOMEDS_SimanStudy::CheckOut(const _PTR(Study) theTarget)
{
  if (!theTarget) return;
  SALOMEDS_Study* aStudy = dynamic_cast<SALOMEDS_Study*>(theTarget.get());
  /*if (_isLocal) {
    //SALOMEDS::Locker lock; mpv: this may cause dedlock when importData in module calls another SALOMEDS method with lock
    _local_impl->CheckOut(aStudy->GetLocalImpl());
  }
  else*/ _corba_impl->CheckOut(aStudy->GetStudy());
}

void SALOMEDS_SimanStudy::CheckIn(const std::string theModuleName)
{
  /*if (_isLocal) {
    //SALOMEDS::Locker lock; mpv: this may cause dedlock when getModifiedData in module calls another SALOMEDS method with lock
    _local_impl->CheckIn(theModuleName);
  }
  else*/ _corba_impl->CheckIn((char*)theModuleName.c_str());
}

_PTR(Study) SALOMEDS_SimanStudy::getReferencedStudy()
{
  SALOMEDSClient_Study* aStudy = NULL;
  /*if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_Study* aStudy_impl = _local_impl->getReferencedStudy();
    if (!aStudy_impl) return _PTR(Study)(aStudy);
    aStudy = new SALOMEDS_Study(aStudy_impl);
  }
  else {
    SALOMEDS::Study_var aStudy_impl = _corba_impl->getReferencedStudy();
    if (CORBA::is_nil(aStudy_impl)) return _PTR(Study)(aStudy);
    aStudy = new SALOMEDS_Study(aStudy_impl);
  }*/
  SALOMEDS::Study_var aStudy_impl = _corba_impl->getReferencedStudy();
  if (CORBA::is_nil(aStudy_impl)) return _PTR(Study)(aStudy);
  aStudy = new SALOMEDS_Study(aStudy_impl);
  return _PTR(Study)(aStudy);
}

std::string SALOMEDS_SimanStudy::StudyId()
{
  std::string anId;
  /*if (_isLocal) {
    SALOMEDS::Locker lock;
    anId = _local_impl->StudyId();
  }
  else*/ anId = (CORBA::String_var)_corba_impl->StudyId();
  return anId;
}

void SALOMEDS_SimanStudy::StudyId(const std::string theId)
{
  /*if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->StudyId(theId);
  }
  else*/ _corba_impl->StudyId((char*)theId.c_str());
}

std::string SALOMEDS_SimanStudy::ScenarioId()
{
  std::string anId;
  /*if (_isLocal) {
    SALOMEDS::Locker lock;
    anId = _local_impl->ScenarioId();
  }
  else*/ anId = (CORBA::String_var)_corba_impl->ScenarioId();
  return anId;
}

void SALOMEDS_SimanStudy::ScenarioId(const std::string theId)
{
  /*if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->ScenarioId(theId);
  }
  else*/ _corba_impl->ScenarioId((char*)theId.c_str());
}

std::string SALOMEDS_SimanStudy::UserId()
{
  std::string anId;
  /*if (_isLocal) {
    SALOMEDS::Locker lock;
    anId = _local_impl->UserId();
  }
  else*/ anId = (CORBA::String_var)_corba_impl->UserId();
  return anId;
}

void SALOMEDS_SimanStudy::UserId(const std::string theId)
{
  /*if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->UserId(theId);
  }
  else*/ _corba_impl->UserId((char*)theId.c_str());
}

void SALOMEDS_SimanStudy::init_orb()
{
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance();
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  _orb = init(0, 0);
}
