dnl Copyright (C) 2003  CEA/DEN, EDF R&D

AC_DEFUN([CHECK_HTML_GENERATORS],[

#AC_CHECKING(for html generators)
AC_CHECKING(for doxygen)

doxygen_ok=yes

dnl were is doxygen ?

AC_PATH_PROG(DOXYGEN,doxygen) 
	
if test "x$DOXYGEN" = "x"
then
  doxygen_ok=no
  AC_MSG_RESULT(no)
  AC_MSG_WARN(doxygen not found)
else
  dnl AC_SUBST(DOXYGEN)
  AC_MSG_RESULT(yes)
fi

AC_CHECKING(for graphviz)

graphviz_ok=yes

dnl were is graphviz ?

AC_PATH_PROG(DOT,dot) 
	
if test "x$DOT" = "x" ; then
  graphviz_ok=no
  AC_MSG_RESULT(no)
  AC_MSG_WARN(graphviz not found)
else
  AC_MSG_RESULT(yes)
fi

])dnl
dnl
