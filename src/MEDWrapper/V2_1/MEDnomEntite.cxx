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
 * - Nom de la fonction : _MEDnomEntite
 * - Description : fournit le nom associe a un type d'entite MED
 * - Parametres :
 *     - nom_ent (OUT) : le nom de l'entite
 *     - type_ent (IN) : le type de l'entite
 * - Resultat : 0 en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_err 
_MEDnomEntite(char *nom_ent,med_entite_maillage type_ent)
{
   switch(type_ent)
     {
     case MED_NOEUD :
       strcpy(nom_ent,MED_NOM_NOE);
       break;

     case MED_MAILLE :
       strcpy(nom_ent,MED_NOM_MAI);
       break;

     case MED_FACE :
       strcpy(nom_ent,MED_NOM_FAC);
       break;

     case MED_ARETE :
       strcpy(nom_ent,MED_NOM_ARE);
       break;

     default :
       return -1;
     }
   return 0;
}

}
