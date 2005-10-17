
#ifndef _CASCatch_CatchSignals_HeaderFile
#define _CASCatch_CatchSignals_HeaderFile

#include <Standard.hxx>

/*!
 * \class CASCatch_CatchSignals
 * \brief This class controls an exception handling
 *
 */ 
class CASCatch_CatchSignals  {

public:

 // Methods PUBLIC
 // 
Standard_EXPORT CASCatch_CatchSignals();
Standard_EXPORT void Destroy() ;
~CASCatch_CatchSignals() { Destroy(); }
Standard_EXPORT void Activate() ;
Standard_EXPORT void Deactivate() ;

private:

/*!\var mySigStates[7], private
 * \brief stores signals' handler functions
 */ 
Standard_Address mySigStates[7];

/*!\var myFloatOpWord
 * \brief stores a float operation word, private
 */ 
Standard_Integer myFloatOpWord;

/*!\var myIsActivated
 * \brief stores a flag whether a catcher is activated, private]
 */
Standard_Boolean myIsActivated;

};


#endif
