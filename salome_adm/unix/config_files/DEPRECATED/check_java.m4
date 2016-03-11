dnl Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
dnl CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
dnl
dnl See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
dnl

AC_DEFUN([CHECK_JAVA],[
dnl AC_REQUIRE([AC_CANONICAL_SYSTEM])dnl
dnl AC_REQUIRE([AC_PROG_CPP])dnl

AC_CHECKING(for JAVA)

JAVA_INCLUDES=""
JAVA_LIBS=""
JAVA_LDPATH=""

java_ok=no

if test -z $JAVAHOME
then
   AC_MSG_WARN(define JAVAHOME variable to use Java)
else
   java_ok=yes
fi

if  test "x$java_ok" = "xyes"
then
   AC_EXEEXT
   AC_CHECK_PROG(JAVA, java$EXEEXT,found)
   if  test "x$JAVA" = "x"
   then
     java_ok=no
   fi

   AC_CHECK_PROG(JAVAC, javac$EXEEXT,found)
   if  test "x$JAVAC" = "x"
   then
     java_ok=no
   fi
fi

if  test "x$java_ok" = "xyes"
then
    version=`$JAVA -version 2>&1 | grep "java version"`
    case "$version" in
      *1.1.*)
          AC_MSG_RESULT(using jdk1.1 version - jdk1.4 required)
	  java_ok=no;;
      *1.2*)
          AC_MSG_RESULT(using jdk1.2 version - jdk1.4 required)
	  java_ok=no;;
      *1.3*)
          AC_MSG_RESULT(using jdk1.3 version - jdk1.4 required)
	  java_ok=no;;
      *1.4*)
          AC_MSG_RESULT(using jdk1.4 version) ;;
    esac
fi

if  test "x$java_ok" = "xyes"
then
dnl java headers
  LOCAL_INCLUDES=" -I${JAVAHOME}/include  -I${JAVAHOME}/include/linux"
  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $LOCAL_INCLUDES"
  AC_CHECK_HEADER(jni.h,java_ok=yes ,java_ok=no)

  CPPFLAGS="$CPPFLAGS_old"
fi

if  test "x$java_ok" = "xyes"
then
dnl libjava library
  LOCAL_LIBS="-L${JAVAHOME}/jre/lib/i386 -ljava -L${JAVAHOME}/jre/lib/i386/client -ljvm -lverify"
  LIBS_old="$LIBS"
  LIBS="$LIBS $LOCAL_LIBS"

  AC_CHECK_LIB(jvm,JNI_CreateJavaVM,java_ok=yes,java_ok=no)

  LIBS="$LIBS_old"
fi

if  test "x$java_ok" = "xyes"
then
  JAVA_INCLUDES="$LOCAL_INCLUDES"
  JAVA_LIBS="$LOCAL_LIBS"
fi

AC_SUBST(JAVA_INCLUDES)
AC_SUBST(JAVA_LIBS)
AC_SUBST(JAVA_LDPATH)

AC_MSG_RESULT(for java: $java_ok)

])dnl
dnl
