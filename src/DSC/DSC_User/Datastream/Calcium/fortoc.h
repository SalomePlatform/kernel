
#ifndef FORTRAN_H
#define FORTRAN_H

/* Operateur de concatenation */
#define  _(A,B)   A##B

#ifdef  __linux
#define F_FUNC(lname,uname) _(lname,_)        /* Fortran function name */
#define F_CALL(lname,uname) _(lname,_)        /* Fortran function call */
#define STR_PSTR(str)       char *str         /* fortran string arg pointer */
#define STR_PLEN(str)       , int _(Len,str)  /* fortran string arg length */
#define STR_PTR(str)        str               /* fortran string pointer */
#define STR_LEN(str)        _(Len,str)        /* fortran string length */


#endif

#endif

