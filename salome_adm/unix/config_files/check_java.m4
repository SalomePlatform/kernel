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
