#ifndef _CASCATCH_FAILURE_HeaderFile
#define _CASCATCH_FAILURE_HeaderFile

#include <Standard_Transient.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( CASCatch_Failure, Standard_Transient )

#include <Standard.hxx>

/*!
 * \class CASCatch_Failure
 * \brief This class presents an exception to be thrown
 *
 */    
class CASCatch_Failure :  public Standard_Transient
{

public:

Standard_EXPORT CASCatch_Failure();
Standard_EXPORT CASCatch_Failure(const Standard_CString aString);
Standard_EXPORT void Reraise(const Standard_CString aMessage) ;
Standard_EXPORT Standard_CString GetError() const;
Standard_EXPORT static  Handle_CASCatch_Failure Caught() ;
Standard_EXPORT static  void Raise(const Standard_CString aMessage = "") ;
Standard_EXPORT virtual  void Throw() const;public:

public:

DEFINE_STANDARD_RTTI( CASCatch_Failure )  

private: 
/*!\var myMessage
 * \brief stores an exception message
 */   
Standard_CString myMessage;

};


#endif
