/* Exception */
#include <iostream.h>

class HDFexception
{
public :
  HDFexception(const char *message) { 
    cerr << message << endl;
  }
};
