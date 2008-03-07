AC_DEFUN([CHECK_LIBXML],[

AC_CHECKING(for libxml library)

AC_SUBST(LIBXML_INCLUDES)
AC_SUBST(LIBXML_LIBS)

LIBXML_INCLUDES=""
LIBXML_LIBS=""

libxml_ok=no

LOCAL_INCLUDES=""
LOCAL_LIBS=""

if test "x$LIBXML_DIR" != "x"
then
  LOCAL_INCLUDES="-I$LIBXML_DIR/include/libxml2"
  if test "x$LIBXML_DIR" = "x/usr"
  then
    LOCAL_LIBS="-lxml2"
  else
    LOCAL_LIBS="-L$LIBXML_DIR/lib -lxml2"
  fi
else
  LOCAL_INCLUDES="-I/usr/include/libxml2"
  LOCAL_LIBS="-lxml2"
fi

dnl libxml2 headers

CPPFLAGS_old="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $LOCAL_INCLUDES"
AC_CHECK_HEADER(libxml/parser.h,libxml_ok="yes",libxml_ok="no")
CPPFLAGS="$CPPFLAGS_old"

if  test "x$libxml_ok" = "xyes"
then

dnl libxml2 library

  LIBS_old=$LIBS
  LIBS="$LIBS $LOCAL_LIBS"
  AC_CHECK_LIB(xml2,xmlInitParser,libxml_ok="yes",libxml_ok="no",)
  LIBS=$LIBS_old
fi

if test "x$libxml_ok" = "xyes"
then
  LIBXML_INCLUDES="$LOCAL_INCLUDES"
  LIBXML_LIBS="$LOCAL_LIBS"
fi

AC_MSG_RESULT(for libxml: $libxml_ok)

])dnl
dnl

