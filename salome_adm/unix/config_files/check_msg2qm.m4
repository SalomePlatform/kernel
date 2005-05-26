# Check availability of Salome's KERNEL binary distribution
#
# Author : Jerome Roy (CEA, 2003)
#

AC_DEFUN([CHECK_MSG2QM],[

AC_CHECKING(for msg2qm)

msg2qm_ok=no

AC_ARG_WITH(msg2qm,
	    [  --with-msg2qm=DIR               root directory path of MSG2QM installation],
	    MSG2QM_DIR="$withval",MSG2QM_DIR="")

if test "x$MSG2QM_DIR" == "x" ; then

# no --with-MSG2QM-dir option used

   if test "x$MSG2QM_ROOT" != "x" ; then

    # MSG2QM_ROOT environment variable defined
      MSG2QM_DIR=$MSG2QM_ROOT

   else

    # search MSG2QM binaries in PATH variable
      AC_PATH_PROG(TEMP, mgs2qm)
      if test "x$TEMP" != "x" ; then
         MSG2QM_BIN_DIR=`dirname $TEMP`
         MSG2QM_DIR=`dirname $MSG2QM_BIN_DIR`
      fi
      
   fi
# 
fi

if test -f ${MSG2QM_DIR}/msg2qm ; then
   msg2qm_ok=yes
   AC_MSG_RESULT(Using MSG2QM executable in ${MSG2QM_DIR})
else
   AC_MSG_WARN("Cannot find MSG2QM executable")
fi

AC_MSG_RESULT(for MSG2QM: $msg2qm_ok)
 
])dnl
 
