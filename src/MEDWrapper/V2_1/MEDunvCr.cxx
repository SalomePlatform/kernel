/*************************************************************************
* COPYRIGHT (C) 1999 - 2002  EDF R&D
* THIS LIBRARY IS FREE SOFTWARE; YOU CAN REDISTRIBUTE IT AND/OR MODIFY
* IT UNDER THE TERMS OF THE GNU LESSER GENERAL PUBLIC LICENSE 
* AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION; 
* EITHER VERSION 2.1 OF THE LICENSE, OR (AT YOUR OPTION) ANY LATER VERSION.
*  
* THIS LIBRARY IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT
* WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
* MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. SEE THE GNU
* LESSER GENERAL PUBLIC LICENSE FOR MORE DETAILS.
*
* YOU SHOULD HAVE RECEIVED A COPY OF THE GNU LESSER GENERAL PUBLIC LICENSE
* ALONG WITH THIS LIBRARY; IF NOT, WRITE TO THE FREE SOFTWARE FOUNDATION,
* INC., 59 TEMPLE PLACE, SUITE 330, BOSTON, MA 02111-1307 USA
*
*************************************************************************/

#include "med.hxx"
#include "med_outils.hxx"

#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <time.h>

#ifdef PPRO_NT
// Windows Header Files:
#include <windows.h>
#include <Lmcons.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

namespace med_2_1{

med_err 
MEDunvCr(med_idt fid, char *maa)
{
  med_idt maaid;
  char chemin [MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  char nomu   [MED_TAILLE_LNOM+1];    
  time_t  temps;
#ifdef PPRO_NT
  struct timeb   tp;
  char   lpBuffer [UNLEN+1];
  long   nSize   = UNLEN+1;
#else
  struct timeval tp;
#endif 
  med_err ret;

  /*
   * On inhibe le gestionnaire d'erreur
   */
  _MEDmodeErreurVerrouiller();

  /*
   * Si le maillage n'existe pas => erreur
   */
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  if ((maaid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
      return -1;

  /*
   * Creation/Ecriture de l'attribut nom universel 
   */
  
#ifdef PPRO_NT
  if ( GetUserName(lpBuffer,&nSize) == 0 ) return -1;
  if ( nSize > MED_TAILLE_NOM ) nSize = MED_TAILLE_NOM;
  strncpy(nomu,lpBuffer,nSize);
  strcat(nomu," ");
  temps=time(&temps);
  strcat(nomu,ctime(&temps));
  ftime(&tp);
  nSize = strlen(nomu)-1;
  if ( sprintf(&nomu[nSize]," %hu",tp.millitm) < 0 ) return -1;
#else
  if (cuserid(nomu) == (void*) NULL) return -1;
  strcat(nomu," ");
  temps=time(&temps);
  strcat(nomu,ctime(&temps));
  if ( gettimeofday(&tp,NULL) < 0 ) return -1;
  if ( sprintf(&nomu[strlen(nomu)-1]," %li",tp.tv_usec) < 0 ) return -1;
#endif
  if ((ret = _MEDattrStringEcrire(maaid,MED_NOM_UNV,MED_TAILLE_LNOM,nomu,MED_REMP)) < 0) 
    return -1;

  /* 
   * Nettoyages divers
   */
  if ((ret = _MEDdatagroupFermer(maaid)) < 0)
    return -1;
 
  return 0;
}

}
