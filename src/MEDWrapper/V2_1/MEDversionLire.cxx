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

namespace med_2_1{

med_err 
MEDversionLire(med_idt fid, med_int *majeur, med_int *mineur, med_int *release) 
{
  med_err ret = 0;
  med_idt gid;  

  /* On ouvre le group ou se trouvent les infos */
  if ((gid = _MEDdatagroupOuvrir(fid,MED_NOM_INFOS)) < 0) {
    *majeur = 2;
    *mineur = -1;
    *release = -1;
    ret = 0;
  }
  else {
    if ((ret = _MEDattrEntierLire(gid,MED_NOM_MAJEUR,majeur)) < 0)
      return -1;

    if ((ret = _MEDattrEntierLire(gid,MED_NOM_MINEUR,mineur)) < 0)
      return -1;

    if ((ret = _MEDattrEntierLire(gid,MED_NOM_RELEASE,release)) < 0)
      return -1;

    /* On ferme tout */
    if ((ret = _MEDdatagroupFermer(gid)) < 0)
      return -1;
  }							

  return ret;
}

}
