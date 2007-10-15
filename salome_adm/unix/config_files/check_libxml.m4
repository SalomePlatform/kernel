AC_DEFUN([CHECK_LIBXML],[

AC_CHECKING(for libxml)

# Custom location of libxml2 package can be specified
# through LIBXML_DIR variable
if test "x$LIBXML_DIR" != "x"
then
  CPPFLAGS="$CPPFLAGS -I$LIBXML_DIR/include/libxml2"
  TMPLIBS="-L$LIBXML_DIR/lib -lxml2 $LIBS"
else
  CPPFLAGS="$CPPFLAGS -I/usr/include/libxml2"
  TMPLIBS="-lxml2 $LIBS"
fi

AC_CHECK_HEADER(libxml/parser.h,libxml_ok="yes",libxml_ok="no")

if  test "x$libxml_ok" = "xyes"
then
  LIBS_old=$LIBS
  LIBS=$TMPLIBS
  AC_CHECK_LIB(xml2,xmlInitParser,libxml_ok="yes",libxml_ok="no",)
  LIBS=$LIBS_old
fi

if test "x$libxml_ok" = "xyes"
then
  LIBS=$TMPLIBS
fi

AC_MSG_RESULT(for libxml: $libxml_ok)

LIBXML_LIBS=$LIBS
AC_SUBST(LIBXML_LIBS)

])dnl
dnl

