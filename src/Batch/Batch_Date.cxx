/*
 * Date.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Nov 26 14:11:42 2003
 * Projet : Salome 2
 *
 */

#include <cstdio>
#include <ctime>
#include "Batch_Date.hxx"
using namespace std;

namespace Batch {

  Date::Date(const long l)
  {
    struct tm * p_tm = localtime(&l);
    _day   = p_tm->tm_mday;
    _month = p_tm->tm_mon  + 1;
    _year  = p_tm->tm_year + 1900;
    _hour  = p_tm->tm_hour;
    _min   = p_tm->tm_min;
    _sec   = p_tm->tm_sec;
  }

  Date::Date(const string s)
  { 
    if ((s == "now") ||	(s == "Now") || (s == "NOW")) {
      long l = time(0);
      struct tm * p_tm = localtime(&l);
      _day   = p_tm->tm_mday;
      _month = p_tm->tm_mon  + 1;
      _year  = p_tm->tm_year + 1900;
      _hour  = p_tm->tm_hour;
      _min   = p_tm->tm_min;
      _sec   = p_tm->tm_sec;

    } else {
      char c;
//       istringstream ist(s);
//       ist >> _day   >> c
// 	  >> _month >> c
// 	  >> _year  >> c
// 	  >> _hour  >> c
// 	  >> _min   >> c
// 	  >> _sec;
      sscanf(s.c_str(), "%ld/%ld/%ld-%ld:%ld:%ld", &_day, &_month, &_year, &_hour, &_min, &_sec);
    }
  }

  Date & Date::operator =(long l)
  {
    struct tm * p_tm = localtime(&l);
    _day   = p_tm->tm_mday;
    _month = p_tm->tm_mon  + 1;
    _year  = p_tm->tm_year + 1900;
    _hour  = p_tm->tm_hour;
    _min   = p_tm->tm_min;
    _sec   = p_tm->tm_sec;

    return *this;
  }

  Date & Date::operator +(long l)
  {
    *this = epoch() + l;
    return *this;
  }

  Date & Date::operator -(long l)
  {
    *this = epoch() - l;
    return *this;
  }

  Date & Date::operator +=(long l)
  {
    *this = epoch() + l;
    return *this;
  }

  Date & Date::operator -=(long l)
  {
    *this = epoch() - l;
    return *this;
  }

  Date & Date::operator =(const string & s)
  {
    if ((s == "now") ||	(s == "Now") || (s == "NOW")) {
      long l = time(0);
      struct tm * p_tm = localtime(&l);
      _day   = p_tm->tm_mday;
      _month = p_tm->tm_mon  + 1;
      _year  = p_tm->tm_year + 1900;
      _hour  = p_tm->tm_hour;
      _min   = p_tm->tm_min;
      _sec   = p_tm->tm_sec;

    } else {
      char c;
//       istringstream ist(s);
//       ist >> _day   >> c
// 	  >> _month >> c
// 	  >> _year  >> c
// 	  >> _hour  >> c
// 	  >> _min   >> c
// 	  >> _sec;
      sscanf(s.c_str(), "%ld/%ld/%ld-%ld:%ld:%ld", &_day, &_month, &_year, &_hour, &_min, &_sec);
    }
 
    return *this;
  }

  string Date::str() const
  {
    char buf[64];
    string datestr;

    // _day to char * 
    sprintf(buf, "%02ld", _day);
    datestr += buf;
    datestr += "/";

    // _month to char * 
    sprintf(buf, "%02ld", _month);
    datestr += buf;
    datestr += "/";

    // _year to char * 
    sprintf(buf, "%04ld", _year);
    datestr += buf;
    datestr += "-";

    // _hour to char * 
    sprintf(buf, "%02ld", _hour);
    datestr += buf;
    datestr += ":";

    // _min to char * 
    sprintf(buf, "%02ld", _min);
    datestr += buf;
    datestr += ":";

    // _sec to char * 
    sprintf(buf, "%02ld", _sec);
    datestr += buf;

    return datestr;
  }

  long Date::epoch() const
  {
    struct tm T;
    T.tm_mday = _day;
    T.tm_mon  = _month - 1;
    T.tm_year = _year  - 1900;
    T.tm_hour = _hour;
    T.tm_min  = _min;
    T.tm_sec  = _sec;
    return mktime(&T);
  }

}


// COMMENTS
