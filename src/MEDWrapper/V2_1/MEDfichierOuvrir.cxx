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
 * - Nom de la fonction : _MEDfichierOuvrir
 * - Description : ouverture d'un fichier HDF en fonction du mode passe
 *                 en parametre
 * - Parametres :
 *     - nom  (IN) : le nom du fichier
 *     - mode (IN) : mode d'ouverture  
 * - Resultat : ID du fichier en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_idt 
_MEDfichierOuvrir(char *nom,med_mode_acces mode)
{ 
  med_idt fid;
  int hdf_mode;

  switch(mode)
    {
    case MED_ECRI :
      hdf_mode = H5F_ACC_RDWR; 
      break;

    case MED_LECT :
      hdf_mode = H5F_ACC_RDONLY;
      break;

    default :
      return -1;
    }  

  if ((fid = H5Fopen(nom,hdf_mode,H5P_DEFAULT)) < 0)
    return -1;

  return fid;
}

}
