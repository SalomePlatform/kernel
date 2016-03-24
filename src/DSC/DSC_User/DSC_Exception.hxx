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

//  File   : DSC_Exception.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
//
#ifndef DSC_EXCEPTION_HXX
#define DSC_EXCEPTION_HXX

#include "Utils_SALOME_Exception.hxx"
#include <string>
#include <iostream>
#include <sstream>
#include <memory>

#include "utilities.h"

#ifndef WIN32
extern "C"
{
#endif
#include <string.h>
#ifndef WIN32
}
#endif


#if defined(_DEBUG_) || defined(_DEBUG)
# ifdef __GNUC__
#  define LOC(message) (message), __FILE__ , __LINE__ , __FUNCTION__
# else
#  define LOC(message) (message), __FILE__, __LINE__
# endif
#else
# define LOC(message)  (message)
#endif


#ifndef SWIG
/**
 * Class OSS is useful when streaming data through a function
 * that expect a string as parameter
 */
class OSS
{
private:
  std::ostringstream oss_;

public:
  explicit OSS() : oss_() {}

  template <class T>
  OSS & operator<<(T obj)
  {
    oss_ << obj;
    return *this;
  }

  operator std::string()
  {
    return oss_.str();
  }

  // Surtout ne pas écrire le code suivant:
  // car oss_.str() renvoie une string temporaire
  //   operator const char*()
  //   {
  //     return oss_.str().c_str();
  //   }

}; /* end class OSS */
#endif


// Cette fonction provient de Utils_SALOME_Exception
// Solution pas très élégante mais contrainte par les manques de la classe SALOME_Exception
const char *makeText( const char *text, const char *fileName, const unsigned int lineNumber);

struct DSC_Exception : public SALOME_Exception {

  // Attention, en cas de modification des paramètres par défaut
  // il est necessaire de les repporter dans la macro DSC_EXCEPTION ci-dessous
  // Le constructeur de la SALOME_Exception demande une chaine non vide
  // Du coup on est obliger de la désallouer avant d'y mettre la notre
  // car le what n'est pas virtuel donc il faut que le contenu de SALOME_Exception::_text
  // soit utilisable.
  // Ne pas mettre lineNumber=0 à cause du calcul log dans la SALOME_Exception si fileName est défini
  DSC_Exception( const std::string & text, 
                 const char *fileName="", 
                 const unsigned int lineNumber=0, 
                 const char *funcName="" ):
    SALOME_Exception(text.c_str()) ,
    _dscText(text),
    _filefuncName(setFileFuncName(fileName?fileName:"",funcName?funcName:"")),
    _lineNumber(lineNumber),
    _exceptionName("DSC_Exception")
  {
    // Mise en cohérence avec l'exception SALOME (à revoir)
    delete [] ((char*)SALOME_Exception::_text);
    if (! _filefuncName.empty() )
      SALOME_Exception::_text = makeText(text.c_str(),_filefuncName.c_str(),lineNumber) ;
    else
      SALOME_Exception::_text = makeText(text.c_str(),0,lineNumber) ;
    
    OSS oss ;
    oss << _exceptionName ;
    if (!_filefuncName.empty() ) oss << " in " << _filefuncName;
    if (_lineNumber) oss << " [" << _lineNumber << "]";
    oss << " : " << _dscText;
    _what = oss;
  }

  virtual const char* what( void ) const throw ()
  {
    return _what.c_str()  ;
  }

  // L'opérateur = de SALOME_Exception n'est pas défini
  // problème potentiel concernant la recopie de son pointeur _text
    
  // Le destructeur de la SALOME_Exception devrait être virtuel
  // sinon pb avec nos attributs de type pointeur.
  virtual ~DSC_Exception(void) throw() {};

  virtual const std::string & getExceptionName() const {return _exceptionName;};

private:

  std::string setFileFuncName(const char * fileName, const char * funcName) {
    ASSERT(fileName);
    ASSERT(funcName);
    OSS oss;
    if ( strcmp(fileName,"") )
      oss << fileName << "##" << funcName;
  
    return oss;
  };

  //DSC_Exception(void) {};
protected:
  std::string  _dscText;
  std::string  _filefuncName;
  int          _lineNumber;
  std::string  _exceptionName;
  std::string  _what;
};

#define DSC_EXCEPTION(Derived) struct Derived : public DSC_Exception { \
  Derived ( const std::string & text, const char *fileName="", const unsigned int lineNumber=0, const char *funcName="" \
            ) : DSC_Exception(text,fileName,lineNumber,funcName) {      \
    _exceptionName = #Derived; \
  } \
    virtual ~Derived(void) throw();\
};\

//Sert à eviter le problème d'identification RTTI des exceptions
//Crée un unique typeInfo pour tous les bibliothèques composants SALOME
//dans un fichier cxx
#define DSC_EXCEPTION_CXX(NameSpace,Derived) NameSpace::Derived::~Derived(void) throw() {};

#endif /* DSC_EXCEPTION_HXX */
