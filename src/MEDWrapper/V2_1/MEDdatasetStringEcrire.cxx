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
 * - Nom de la fonction : _MEDdatasetStringEcrire
 * - Description : ecriture d'un dataset tableau de caracteres
 * - Parametres :
 *     - pere (IN)     : l'ID de l'objet HDF pere ou placer l'attribut
 *     - nom  (IN)     : le nom de l'attribut 
 *     - dimd (IN)     : profil du tableau
 *     - val  (IN)     : valeurs du tableau
 *     - mode (IN)     : mode d'ecriture MED
 * - Resultat : 0 en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_err 
_MEDdatasetStringEcrire(med_idt pere,char *nom,med_size *dimd,
			char *val, med_mode_acces mode)
{
  med_idt dataset;
  med_idt datatype = 0;
  med_idt dataspace = 0;
  med_err ret;

  if ((dataset = H5Dopen(pere,nom)) < 0)
    {
      if ((dataspace = H5Screate_simple(1,dimd,NULL)) < 0)
	return -1;
      if((datatype = H5Tcopy(H5T_C_S1)) < 0)
	return -1;
      if((ret = H5Tset_size(datatype,1)) < 0)
	return -1;
      if ((dataset = H5Dcreate(pere,nom,datatype,dataspace,
			     H5P_DEFAULT)) < 0)
	return -1;    
    }
  else
    if (mode != MED_REMP)
      {
	H5Dclose(dataset);
	return -1;
      }
    else
      {
      if ((dataspace = H5Screate_simple(1,dimd,NULL)) < 0)
	return -1;
      if((datatype = H5Tcopy(H5T_C_S1)) < 0)
	return -1;
      if((ret = H5Tset_size(datatype,1)) < 0)
	return -1;
      }
  if ((ret = H5Dwrite(dataset,datatype,H5S_ALL,H5S_ALL,
		      H5P_DEFAULT, val)) < 0)
    return -1;
  if (dataspace)
    if((ret = H5Sclose(dataspace)) < 0)
      return -1;
  if (datatype)
    if ((ret = H5Tclose(datatype)) < 0)
      return -1;
  if ((ret = H5Dclose(dataset)) < 0)
    return -1;

  return 0;
}

}
