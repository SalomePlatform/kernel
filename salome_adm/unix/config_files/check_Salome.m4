# Check availability of Salome binary distribution
#
# Author : Marc Tajchman (CEA, 2002)
#

AC_DEFUN([CHECK_SALOME],[

AC_CHECKING(for Salome)

Salome_ok=no

AC_ARG_WITH(salome,
	    --with-salome=DIR root directory path of SALOME installation,
	    SALOME_DIR="$withval",SALOME_DIR="")

if test "x$SALOME_DIR" == "x" ; then

# no --with-salome-dir option used

   if test "x$SALOME_ROOT_DIR" != "x" ; then

    # SALOME_ROOT_DIR environment variable defined
      SALOME_DIR=$SALOME_ROOT_DIR

   else

    # search Salome binaries in PATH variable
      AC_PATH_PROG(TEMP, libMEDMEM_Swig.py)
      if test "x$TEMP" != "x" ; then
         SALOME_BIN_DIR=`dirname $TEMP`
         SALOME_DIR=`dirname $SALOME_BIN_DIR`
      fi
      
   fi
# 
fi

if test -f ${SALOME_DIR}/bin/libMEDMEM_Swig.py  ; then
   Salome_ok=yes
   AC_MSG_RESULT(Using Salome distribution in ${SALOME_DIR})

   if test "x$SALOME_ROOT_DIR" == "x" ; then
      SALOME_ROOT_DIR=${SALOME_DIR}
   fi
   if test "x$SALOME_SITE_DIR" == "x" ; then
      SALOME_SITE_DIR=${SALOME_ROOT_DIR}
   fi
   AC_SUBST(SALOME_ROOT_DIR)
   AC_SUBST(SALOME_SITE_DIR)

else
   AC_MSG_WARN("Cannot find compiled Salome distribution")
fi
  
AC_MSG_RESULT(for Salome: $Salome_ok)
 
])dnl
 
