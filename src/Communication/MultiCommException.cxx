#include "MultiCommException.hxx"

MultiCommException::MultiCommException(const char *message)
{
  _message=message;
}

const char *MultiCommException::what() const
{
  return _message.c_str();
}
