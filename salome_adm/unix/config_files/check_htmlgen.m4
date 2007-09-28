dnl Copyright (C) 2003  CEA/DEN, EDF R&D

AC_DEFUN([CHECK_HTML_GENERATORS],[

#AC_CHECKING(for html generators)

doxygen_ok=yes
dnl were is doxygen ?
AC_PATH_PROG(DOXYGEN,doxygen) 
if test "x$DOXYGEN" = "x"
then
  AC_MSG_WARN(doxygen not found)
  doxygen_ok=no
fi
if test "x$doxygen_ok" = "xyes"
then
  version=`$DOXYGEN --version`
  AC_MSG_RESULT(doxygen version $version)
  case "$version" in
	1.4.4*)
	  DOXYGEN_WITH_PYTHON=yes
	  DOXYGEN_WITH_STL=no
	  ;;
	1.4.5*)
	  DOXYGEN_WITH_PYTHON=yes
	  DOXYGEN_WITH_STL=yes
	  ;;
	1.4.6*)
	  DOXYGEN_WITH_PYTHON=yes
	  DOXYGEN_WITH_STL=yes
	  ;;
	1.4.7*)
	  DOXYGEN_WITH_PYTHON=yes
	  DOXYGEN_WITH_STL=yes
	  ;;
	1.4.8*)
	  DOXYGEN_WITH_PYTHON=yes
	  DOXYGEN_WITH_STL=yes
	  ;;
	1.4.9*)
	  DOXYGEN_WITH_PYTHON=yes
	  DOXYGEN_WITH_STL=yes
	  ;;
	[[1-9]].[[5-9]]*)
	  DOXYGEN_WITH_PYTHON=yes
	  DOXYGEN_WITH_STL=yes
	  ;;
	*)
	  DOXYGEN_WITH_PYTHON=no
	  DOXYGEN_WITH_STL=no
	  ;;
  esac
  AC_MSG_RESULT(doxygen with support STL    - $DOXYGEN_WITH_STL)
  AC_MSG_RESULT(doxygen with support PYTHON - $DOXYGEN_WITH_PYTHON)
  AC_SUBST(DOXYGEN_WITH_PYTHON)
  AC_SUBST(DOXYGEN_WITH_STL)
fi
dnl AC_SUBST(DOXYGEN)

graphviz_ok=yes
dnl were is graphviz ?
AC_PATH_PROG(DOT,dot) 
if test "x$DOT" = "x" ; then
  AC_MSG_WARN(graphviz not found)
  graphviz_ok=no
fi
dnl AC_SUBST(DOT)

AC_PATH_PROG(LATEX,latex) 
if test "x$LATEX" = "x" ; then
  AC_MSG_WARN(latex not found)
fi
AC_SUBST(LATEX)

AC_PATH_PROG(DVIPS,dvips)
if test "x$DVIPS" = "x" ; then
  AC_MSG_WARN(dvips not found)
fi
AC_SUBST(DVIPS)

AC_PATH_PROG(PDFLATEX,pdflatex)
if test "x$PDFLATEX" = "x" ; then
  AC_MSG_WARN(pdflatex not found)
fi
AC_SUBST(PDFLATEX)

rst2html_ok=yes
dnl were is rst2html ?
AC_PATH_PROG(RST2HTML,rst2html) 
if test "x$RST2HTML" = "x"; then
  AC_PATH_PROG(RST2HTML,rst2html.py)
fi

if test "x$RST2HTML" = "x"; then
  AC_MSG_WARN(rst2html not found)
  rst2html_ok=no
fi
AC_SUBST(RST2HTML)

AM_CONDITIONAL(RST2HTML_IS_OK, [test x"$rst2html_ok" = xyes])

])dnl
dnl
