/***********************************************************
**  File:    QAD.h
**  Descr:   General definitions for QAD
**  Created: UI team, 20.09.00
************************************************************/
#ifndef QAD_H
#define QAD_H

#include <assert.h>

#if defined QAD_DLL
#if defined WNT
#define QAD_EXPORT  _declspec( dllexport )
#else
#define QAD_EXPORT
#endif
#else
#if defined WNT
#define QAD_EXPORT  _declspec( dllimport )
#else
#define QAD_EXPORT  
#endif
#endif

#if defined SOLARIS
#define bool  int
#define false 0
#define true  1
#endif

#if defined WNT
#pragma warning ( disable: 4251 )
#endif

#if defined (_DEBUG) 
#define QAD_ASSERT(x)			assert(x)
#define QAD_ASSERT_DEBUG_ONLY(x)	assert(x)

#else	/* _DEBUG */
#define QAD_ASSERT(x)			x
#define QAD_ASSERT_DEBUG_ONLY(x)	
#endif

/*  Message box button's IDs */
#define QAD_OK                      1
#define QAD_CANCEL                  2
#define QAD_YES                     3
#define QAD_NO                      4
#define QAD_HELP                    5

#endif
