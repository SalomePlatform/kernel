#ifndef _MULTICOMMEXCEPTION_HXX_
#define _MULTICOMMEXCEPTION_HXX_

#include <string>


class MultiCommException {
private:
  std::string _message;
public:
  MultiCommException(const char *message);
  const char *what() const;
};

#endif

