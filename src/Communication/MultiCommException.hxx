#ifndef _MULTICOMMEXCEPTION_HXX_
#define _MULTICOMMEXCEPTION_HXX_

#include <string>

using namespace std;

class MultiCommException {
private:
  string _message;
public:
  MultiCommException(const char *message);
  const char *what() const;
};

#endif

