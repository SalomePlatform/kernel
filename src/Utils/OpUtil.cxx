using namespace std;
#include "utilities.h" 
#include "OpUtil.hxx"
#include <unistd.h>
#include <errno.h>
#include <string.h>

int gethostname(char *name, size_t len);

string GetHostname()
{
  int ls = 100, r = 0;
  char *s;

  while (ls < 10000) {
    ls *= 2;
    s = new char[ls];
    r = gethostname(s, ls-1);
    switch (r) 
      {
      case 0:
	  break;
      default:
#ifdef EINVAL
      case EINVAL:
#endif
#ifdef ENAMETOOLONG
      case ENAMETOOLONG:
#endif
        delete [] s;
	continue;
      }
  }

  if (r != 0) {
    s = new char[50];
    strcpy(s, "localhost");
  }

  // remove all after '.'
  char *aDot = (strchr(s,'.'));
  if (aDot) aDot[0] = '\0';

  string p = s;
  delete [] s;
  return p;
}

