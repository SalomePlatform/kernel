//  
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
//  File   : 
//  Author : 
//  Module : 
//  $Header$

#ifndef MED_Common_HeaderFile
#define MED_Common_HeaderFile

#include <string>
#include <vector>
#include <set>
#include <map>

extern "C"{
#include <hdf5.h>
}  

#include <boost/shared_ptr.hpp>

namespace MED{

  template<class T> class shared_ptr: public boost::shared_ptr<T>
  {
  public:
    shared_ptr() {}

    template<class Y>
    explicit shared_ptr(Y * p): boost::shared_ptr<T>(p) {}

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r): boost::shared_ptr<T>(r) {}

    operator const T& () const { return *get();}

    operator T& () { return *get();}
  };


  typedef enum {eFAUX, eVRAI} EBooleen ; 
  typedef double TFloat;
  typedef int TInt;

  typedef hid_t TIdt;
  typedef herr_t TErr;

  typedef enum {eFLOAT64=6, eINT=26} ETypeChamp;

  typedef enum {eNON_STRUCTURE, eSTRUCTURE} EMaillage;

  typedef enum {eCART, eCYL, eSPHER} ERepere; 

  typedef enum {eNOD, eDESC} EConnectivite ; 

  typedef enum {ePOINT1=1, eSEG2=102, eSEG3=103, eTRIA3=203,
		eQUAD4=204, eTRIA6=206,eQUAD8=208, eTETRA4=304,
		ePYRA5=305, ePENTA6=306, eHEXA8=308, eTETRA10=310, 
		ePYRA13=313, ePENTA15=315, eHEXA20=320, 
		ePOLYGONE=400, ePOLYEDRE=500, eNONE=0} EGeometrieElement;

  typedef enum {eMAILLE, eFACE, eARETE, eNOEUD} EEntiteMaillage; 

  typedef enum {eNO_PFLMOD, eGLOBAL, eCOMPACT}  EModeProfil; 

  typedef std::vector<TFloat> TFloatVector;
  typedef std::vector<std::string> TStringVector;
  typedef std::vector<TInt> TIntVector;
  typedef std::set<std::string> TStringSet;
  
  typedef std::map<EGeometrieElement,TInt> TGeom;
  typedef std::map<EEntiteMaillage,TGeom> TEntityInfo;

  typedef std::set<EGeometrieElement> TGeomSet;
  typedef std::map<EEntiteMaillage,TGeomSet> TEntity2GeomSet;

  const TEntity2GeomSet& GetEntity2GeomSet();

  inline TInt GetNbConn(EEntiteMaillage theEntity, 
			EGeometrieElement theElem,
			TInt theMeshDim)
  {
    TInt anElemDim = theElem / 100, nsup = 0;
    if(theEntity == eMAILLE && anElemDim < theMeshDim) nsup = 1;
    return nsup + theElem % 100;
  }

  struct TNameInfo;
  typedef MED::shared_ptr<TNameInfo> PNameInfo;
  
  struct TMeshInfo;
  typedef MED::shared_ptr<TMeshInfo> PMeshInfo;
  
  struct TFamilyInfo;
  typedef MED::shared_ptr<TFamilyInfo> PFamilyInfo;

  struct TElemInfo;
  typedef MED::shared_ptr<TElemInfo> PElemInfo;

  struct TNodeInfo;
  typedef MED::shared_ptr<TNodeInfo> PNodeInfo;

  struct TCellInfo;
  typedef MED::shared_ptr<TCellInfo> PCellInfo;

  struct TFieldInfo;
  typedef MED::shared_ptr<TFieldInfo> PFieldInfo;

  struct TTimeStampInfo;
  typedef MED::shared_ptr<TTimeStampInfo> PTimeStampInfo;
  
  struct TTimeStampVal;
  typedef MED::shared_ptr<TTimeStampVal> PTimeStampVal;

  class TWrapper;
  typedef MED::shared_ptr<TWrapper> PWrapper;
};


#endif
