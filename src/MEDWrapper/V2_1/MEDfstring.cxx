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
#include <string.h>

/*
 *  Chaine C -> chaine FORTRAN completee par des blancs
 */

namespace med_2_1{

med_err
_MEDfstring(char *chaine, med_int longueur_fixee)
{
  int longueur_reelle, i;

  if (longueur_fixee == 0 ) return 0;

  longueur_reelle = strlen(chaine);
  if (longueur_fixee < longueur_reelle)
    return -1;

  /* on supprime le caractere de fin de chaine C '\0'
     et complete par des blancs */
  for (i=longueur_reelle;i<longueur_fixee;i++)
    *(chaine+i) = ' ';

  return 0;
}

}
