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

/*
 * - Nom de la fonction : _MEDobjetIdentifier
 * - Description : retrouve le nom de l'objet de rang "indice" 
 *                 se trouvant dans le datagroup "chemin"
 * - Parametres :
 *     - fid     (IN)     : l'ID du fichier ou se trouve le datagroup
 *     - chemin  (IN)     : chemin d'acces au datagroup
 *     - indice  (IN)     : indice de l'objet du datagroup dont on veut
 *                          le nom
 *     - nom     (OUT)    : le nom 
 * - Resultat : 0 en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_err 
_MEDobjetIdentifier(med_idt fid,char *chemin,int indice,void *nom)
{
  int idx;

  if ((idx = H5Giterate(fid,chemin,&indice,_MEDindiceInfo,
			nom)) < 0)
    return -1;

  return 0;
}

}
