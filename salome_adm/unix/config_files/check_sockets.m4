AC_DEFUN([CHECK_SOCKETS],[

dnl Author

dnl Warren Young <warren@etr-usa.com>
dnl M4 Source Code

echo
echo ---------------------------------------------
echo testing sockets
echo ---------------------------------------------
echo

AC_CACHE_CHECK(for libraries containing socket functions,
ac_cv_socket_libs, [
	oCFLAGS=$CFLAGS

        AC_TRY_LINK([
                        #include <sys/types.h>
                        #include <sys/socket.h>
                        #include <netinet/in.h>
                        #include <arpa/inet.h>
                ],
                [
                        struct in_addr add;
                        int sd = socket(AF_INET, SOCK_STREAM, 0);
                        inet_ntoa(add);
                ],
                ac_cv_socket_libs=-lc, ac_cv_socket_libs=no)

        if test x"$ac_cv_socket_libs" = "xno"
        then
                CFLAGS="$oCFLAGS -lsocket"
                AC_TRY_LINK([
                                #include <sys/types.h>
                                #include <sys/socket.h>
                                #include <netinet/in.h>
                                #include <arpa/inet.h>
                        ],
                        [
                                struct in_addr add;
                                int sd = socket(AF_INET, SOCK_STREAM, 0);
                                inet_ntoa(add);
                        ],
                        ac_cv_socket_libs=-lsocket, ac_cv_socket_libs=no)
        fi

        if test x"$ac_cv_socket_libs" = "xno"
        then
                CFLAGS="$oCFLAGS -lsocket -lnsl"
                AC_TRY_LINK([
                                #include <sys/types.h>
                                #include <sys/socket.h>
                                #include <netinet/in.h>
                                #include <arpa/inet.h>
                        ],
                        [
                                struct in_addr add;
                                int sd = socket(AF_INET, SOCK_STREAM, 0);
                                inet_ntoa(add);
                        ],
                        ac_cv_socket_libs="-lsocket -lnsl", ac_cv_socket_libs=no)
        fi

        CFLAGS=$oCFLAGS
])

        if test x"$ac_cv_socket_libs" = "xno"
        then
                AC_MSG_ERROR([Cannot find socket libraries])
        elif test x"$ac_cv_socket_libs" = "x-lc"
        then
                SOCKETLIBS=""
                CPPFLAGS="-DHAVE_SOCKET $CPPFLAGS"
     	        CORBA_IDLCXXFLAGS="-DHAVE_SOCKET $CORBA_IDLCXXFLAGS"
 		CORBA_IDLPYFLAGS="-DHAVE_SOCKET $CORBA_IDLPYFLAGS"
        else
                SOCKETLIBS="$ac_cv_socket_libs"
                CPPFLAGS="-DHAVE_SOCKET $CPPFLAGS"
     	        CORBA_IDLCXXFLAGS="-DHAVE_SOCKET $CORBA_IDLCXXFLAGS"
 		CORBA_IDLPYFLAGS="-DHAVE_SOCKET $CORBA_IDLPYFLAGS"
        fi

        AC_SUBST(SOCKETLIBS)
        AC_SUBST(SOCKETFLAGS)
]) dnl CHECK_SOCKET
