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


#include <stdlib.h>
#include "med_misc.hxx"

/*
 * - Nom de la fonction : _MED1cstring
 * - Description : convertit une chaine de caracteres FORTRAN
 *                 en une nouvelle chaine de caracteres C
 *                 dont la longueur est passee en parametre.
 *                 Les caracteres completes sont des blancs
 * - Parametres :
 *     - chaine (IN)          : la chaine FORTRAN
 *     - longueur_reelle (IN) : la longueur de la chaine FORTRAN
 *     - longueur_fixee (IN)  : longueur de la chaine C a construire
 * - Resultat : la nouvelle chaine C en cas de succes, NULL sinon
 */

namespace med_2_1{

char *
_MED1cstring(char *chaine,int longueur_reelle,int longueur_fixee)
{
  char *nouvelle;
  int i;

  if (longueur_reelle > longueur_fixee)
    return NULL;

  if ((nouvelle = (char *) malloc(sizeof(char)*(longueur_fixee+1))) == NULL)
    return NULL;

  for (i=0;i<longueur_reelle;i++)
    *(nouvelle+i) = *(chaine+i);

  for (i=longueur_reelle;i<longueur_fixee;i++)
    *(nouvelle+i) = ' ';
  
  *(nouvelle+longueur_fixee) = '\0';

  return nouvelle;
}

}
