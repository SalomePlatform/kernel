dnl @synopsis AC_CXX_HAVE_SSTREAM
dnl
dnl If the C++ library has a working stringstream, define HAVE_SSTREAM.
dnl
dnl @author Ben Stanley
dnl @version $Id$
dnl
dnl modified by Marc Tajchman (CEA) - 10/10/2002
dnl
AC_DEFUN([AC_CXX_HAVE_SSTREAM],
[AC_CACHE_CHECK(whether the compiler has stringstream,
HAVE_SSTREAM,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <sstream>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[stringstream message; message << "Hello"; return 0;],
 HAVE_SSTREAM=yes, HAVE_SSTREAM=no)
 AC_LANG_RESTORE
])
AC_SUBST(HAVE_SSTREAM)
])
