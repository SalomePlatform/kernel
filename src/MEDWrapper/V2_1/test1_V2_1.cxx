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

/******************************************************************************
 * - Nom du fichier : test1.c
 *
 * - Description : tests des routines d'ouverture/fermeture des
 *                 fichiers MED 
 *
 *****************************************************************************/

#include "med.hxx"
using namespace med_2_1;

/******************************************************************************
 * OUVERTURE/FERMETURE DE FICHIERS :
 *
 * Description :
 *    - ouverture du fichier en mode ecriture avec remplacement
 *    - ecriture d'une description du fichier (optionnel)
 *    - fermeture du fichier
 *    - ouverture du fichier en mode de lecture
 *    - fermeture du fichier
 *****************************************************************************/

int main (int argc, char **argv)
{
  med_err ret = 0;
  med_idt fid;
  char des[MED_TAILLE_DESC+1]="Ceci est un courte description du mon fichier test1.med";

  fid = MEDouvrir("test1.med",MED_REMP);
  if (fid < 0)
    ret = -1;
  printf("%d\n",ret);

  if (ret == 0)
    ret = MEDfichDesEcr(fid,des, MED_REMP);
  printf("%d\n",ret);

  ret = MEDfermer(fid);
  printf("%d\n",ret);

  fid = MEDouvrir("test1.med",MED_LECT);
  if (fid < 0)
    ret = -1;
  printf("%d\n",ret);

  ret = MEDfermer(fid);
  printf("%d\n",ret);
  
  return 0;
}




