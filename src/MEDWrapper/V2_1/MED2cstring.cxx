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
 * - Nom de la fonction _MED2cstring
 * - Description : convertit une chaine de caracteres FORTRAN en 
 *                 nouvelle chaine de caracteres C
 * - Parametres :
 *     - chaine (IN)   : la chaine FORTRAN
 *     - longueur (IN) : longueur de la chaine
 * - Resultat : la nouvelle chaine C en cas de succes, NULL sinon
 */

namespace med_2_1{

char *
_MED2cstring(char *chaine, int longueur)
{
  char *nouvelle;
  char *temoin;
  int long_reelle = longueur;
  int i;

  if ( longueur < 0 ) return NULL;

  temoin = chaine+longueur-1;
  while (*temoin == ' ' && (temoin != chaine) )
    {
      temoin --;
      long_reelle--;
    }
  if ( *temoin == ' ') long_reelle = 0;
      
  if ((nouvelle = (char *) malloc(sizeof(char)*(long_reelle+1))) == NULL)
    return NULL;

  for (i=0;i<long_reelle+1;i++)
    *(nouvelle+i) = *(chaine+i);
  *(nouvelle+long_reelle) = '\0';

  return nouvelle;
}

}
