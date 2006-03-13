/*
 * IOMutex.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 14 11:00:39 2003
 * Projet : Salome 2
 *
 */

#ifndef _IOMUTEX_H_
#define _IOMUTEX_H_

#include <pthread.h>

#define LOCK_IO               { pthread_mutex_lock(&Batch::IOMutex) ; }
#define UNLOCK_IO             { pthread_mutex_unlock(&Batch::IOMutex) ; }
#define UNDER_LOCK(statement) { LOCK_IO ; { statement ; } ; UNLOCK_IO ; }

namespace Batch {

  extern pthread_mutex_t IOMutex;
  
}

#endif
