#ifndef _MULTICOMMEXCEPTION_HXX_
#define _MULTICOMMEXCEPTION_HXX_

#include <string>

#if defined WNT && defined COMMUNICATION_EXPORTS
#define COMMUNICATION_EXPORT __declspec( dllexport )
#else
#define COMMUNICATION_EXPORT
#endif

class COMMUNICATION_EXPORT MultiCommException {
private:
  std::string _message;
public:
  MultiCommException(const char *message);
  const char *what() const;
};

#endif

