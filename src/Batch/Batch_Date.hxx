/*
 * Date.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Nov 26 14:11:42 2003
 * Projet : Salome 2
 *
 */

#ifndef _DATE_H_
#define _DATE_H_

using namespace std;
#include <string>

namespace Batch {

  class Date
  {
  public:
    Date(const long l=0);
    Date(const string s);
    virtual Date & operator =(long l);
    virtual Date & operator +(long l);
    virtual Date & operator -(long l);
    virtual Date & operator +=(long l);
    virtual Date & operator -=(long l);
    virtual Date & operator =(const string & s);
    virtual string str() const;
    virtual long epoch() const;

  protected:
    int _day, _month, _year;
    int _hour, _min, _sec;

  private:

  };

}

#endif

// COMMENTS
