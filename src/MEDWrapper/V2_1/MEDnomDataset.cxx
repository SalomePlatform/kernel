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
 * - Nom de la fonction : _MEDnomDataset
 * - Description : fournit un nom de dataset
 * - Parametres :
 *     - nom_dataset (OUT) : le nom du data set
 *     - quoi (IN)         : le type de table MED
 *     - type_conn (IN)    : le type de connectivite
 * - Resultat : 0 en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_err 
_MEDnomDataset(char *nom_dataset,med_table quoi,med_connectivite type_conn)
{
  switch(quoi)
    {
    case MED_COOR :
      strcpy(nom_dataset,MED_NOM_COO);
      break;

    case MED_CONN :
      switch(type_conn)
	{
	case MED_NOD :
	  strcpy(nom_dataset,MED_NOM_NOD);
	  break;

	case MED_DESC :
	  strcpy(nom_dataset,MED_NOM_DES);
	  break;

	default :
	  return -1;
	}
      break;

    case MED_NOM :
      strcpy(nom_dataset,MED_NOM_NOM);
      break;

    case MED_NUM :
      strcpy(nom_dataset,MED_NOM_NUM);
      break;

    case MED_FAM :
      strcpy(nom_dataset,MED_NOM_FAM);
      break;

    default :
      return -1;
    }

  return 0;
}

}
