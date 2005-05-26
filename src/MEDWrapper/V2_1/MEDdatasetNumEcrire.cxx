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
#include <stdlib.h>

/*
 * - Nom de la fonction : _MEDdatasetNumEcrire
 * - Description : ecriture d'un dataset tableau numerique
 * - Parametres :
 *     - pere (IN)      : l'ID de l'objet HDF pere ou placer l'attribut
 *     - nom  (IN)      : le nom du dataset
 *     - type (IN)      : type numerique MED { MED_REEL64 , MED_INT32 , MED_INT64 }
 *     - interlace (IN) : Choix du type d'entrelacement utilisé par l'appelant { MED_FULL_INTERLACE(x1,y1,z1,x2,...)) , MED_NO_INTERLACE(x1,x2,y1,y2,z1,z2) }
 *       - nbdim   (IN) : Dimension des éléments
 *       - fixdim  (IN) : MED_ALL ou n° de la dimension a enregistrer
 *     - psize     (IN) : Taille du profil à utiliser, MED_NOPF si pas de profil 
 *                        (référence les élements, cette taille ne prend pas en compte le nombre de pts de gauss ni la dimension )  
 *       - pfltab  (IN) : Tableau contenant les n° déléments à traiter (1....oo)
 *       - pflmod  (IN) : PARAMETRE A AJOUTER : Indique comment lire les informations en mémoire { MED_COMPACT, MED_GLOBALE }. 
 *       - ngauss  (IN) : Nombre de points de GAUSS par élément
 *     - size (IN)     : Taille du tableau de valeurs
 *                        (référence tous les élements, cette taille  prend en compte le nombre de pts de gauss et la dimension )  
 *     - val  (IN)     : valeurs du tableau
 *     - mode (IN)     : mode d'ecriture MED (MED_ECRI | MED_REMP)
 * - Resultat : 0 en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_err 
_MEDdatasetNumEcrire(med_idt pere,char *nom, med_type_champ type,
		     med_mode_switch interlace, med_size nbdim, med_size fixdim, 
		     med_size psize, med_ssize * pfltab, med_int ngauss,
		     med_size *size,  unsigned char *val, med_mode_acces mode)
{
  med_idt    dataset, dataspace = 0, memspace = 0;
  med_ssize  start_mem[1],start_data[1],*pflmem,*pfldsk;
  med_size   stride[1],count[1],pcount[1],pflsize[1];
  med_err    ret;
  int        i,j,index,type_hdf;
  int        dim, firstdim, dimutil, lastdim ;
  med_mode_profil pflmod;

  /* Verify fixdim is between [0, nbdim] ( 0 is MED_ALL ) */
  if ( ( fixdim < 0 ) || ( fixdim > nbdim ) ) 
    return -1;

  /* block pflmod to MED_COMPACT (until med2.2) */
  pflmod = MED_COMPACT;

  switch(type) 
    {
    case MED_REEL64 :
      /* 1) IA32 is LE but due to an (?HDF convertion BUG?) when using H5T_NATIVE_DOUBLE/MED_REEL64? under PCLINUX
	 the file read under SGI is incorrect
	 2) Compaq OSF/1 is LE, since we force SGI64,SUN4SOL2,HP to write double in LE even if they are BE, mips OSF/1 must be BE
	 REM  : Be careful of compatibility between MED files when changing this (med2.2)                    */
#if defined(PCLINUX) || defined(OSF1)
      type_hdf = H5T_IEEE_F64BE;
#else     
      type_hdf = H5T_IEEE_F64LE;
#endif
      break;

    case MED_INT32 :
#if defined(PCLINUX)
      type_hdf = H5T_STD_I32BE;
      if ((H5Tconvert(H5T_NATIVE_INT,H5T_STD_I32BE,(hsize_t)*size,(void *)val,NULL,NULL)) < 0) 
	  return -1;
#else
      type_hdf = H5T_NATIVE_INT;
#endif
      break;
 
    case MED_INT64 :
      type_hdf = H5T_NATIVE_LONG;
      break;

    default :
      return -1;
    }


  if ((dataset = H5Dopen(pere,nom)) < 0)
    {
      /* Whatever the size of the profil is we create a dataset with the size of the value array               */
      /* Then if we used the MED_REMP mode we can append a new dimension to a previous one in the dataset      */
      /* When we'll use the compression mode, the space used by unused values would be easily compressed       */
  
      if ((dataspace = H5Screate_simple(1,size,NULL)) < 0)
	return -1;
      if ((dataset = H5Dcreate(pere,nom,type_hdf,dataspace,
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
      if ((dataspace = H5Dget_space(dataset)) <0)
	return -1;


  switch(interlace) 
    {  /* switch Interlace */
    case MED_FULL_INTERLACE :
      
      /*Initialisation des indices de boucle du traitement de l'entrelacement en fonction de la dimension fixee*/
      if ( fixdim != MED_ALL) 
	{ 
	  firstdim = fixdim-1;
	  lastdim  = fixdim;
	  dimutil  = 1;
	} else	{
	  firstdim = 0;
	  lastdim  = nbdim;
	  dimutil  = nbdim; 
	}

      count [0] = (*size)/(nbdim);


      if ( psize == MED_NOPF ) {  

	/* Creation d'un data space mémoire de dimension 1, de longeur size, et de longeur maxi size */
	if ( (memspace = H5Screate_simple (1, size, NULL)) <0)
	  return -1;
	
	stride[0] = nbdim;  

	for (dim=firstdim; dim < lastdim; dim++) {
	  
	  start_mem[0] = dim;
	  if ( (ret = H5Sselect_hyperslab (memspace, H5S_SELECT_SET, start_mem, stride, 
					   count, NULL)) <0)
	    return -1; 
	  
	  start_data[0] = dim*count[0];
	  if ( (ret = H5Sselect_hyperslab (dataspace, H5S_SELECT_SET, start_data, NULL, 
					   count, NULL)) <0)
	    return -1; 
	  
	  if ((ret = H5Dwrite(dataset,type_hdf,memspace,dataspace,
			      H5P_DEFAULT, val)) < 0)
	    return -1;
	}
	
      } else { /* psize != MED_NOPF */
	
	pflsize [0] = psize*ngauss*nbdim;
	pcount  [0] = psize*ngauss*dimutil;
	pflmem      = (med_ssize *) malloc (sizeof(med_ssize)*pcount[0]);
	pfldsk      = (med_ssize *) malloc (sizeof(med_ssize)*pcount[0]);
	
	switch(pflmod)
	  { /* switch pflmod pout FULL_INTERLACE*/
	  case MED_GLOBALE :

	    /* Creation d'un data space mémoire de dimension 1, de longeur size, et de longeur maxi size */
	    if ( (memspace = H5Screate_simple (1, size, NULL)) <0)
	      return -1;

	    for (dim=firstdim; dim < lastdim; dim++) {
	      
	      for (i=0; i < psize; i++)              /* i balaye les élements du profil */
		for (j=0; j < ngauss; j++) {         
		  index = i*ngauss+j + (dim-firstdim)*(psize*ngauss);
		  pflmem[index] = (pfltab[i]-1)*ngauss*nbdim + j*nbdim+dim;
		  pfldsk[index] = dim*count[0] + (pfltab[i]-1)*ngauss+j;	     
		}
	    }
	     
	    if ( (ret = H5Sselect_elements(memspace,H5S_SELECT_SET, pcount[0], (const hssize_t **) pflmem ) ) <0) 
	      return -1; 
	      
	    if ( (ret = H5Sselect_elements(dataspace,H5S_SELECT_SET, pcount[0], (const hssize_t **) pfldsk ) ) <0) 
	      return -1; 
	    
	    break;
	    
	  case MED_COMPACT :

	    /* Creation d'un data space mémoire de dimension 1, de la longeur du profil          */
	    /* La dimension utilisée est ici nbdim, même pour un profil compact on suppose       */
	    /*  que l'utilisateur a toutes les coordonées stockées, même si il en demande qu'une */ 

	    if ( (memspace = H5Screate_simple (1, pflsize, NULL)) <0)
	      return -1;
	    
	    for (dim=firstdim; dim < lastdim; dim++) {
	      
	      for (i=0; i < psize; i++)              /* i balaye les élements du profil */
		for (j=0; j < ngauss; j++) {         
		  index = i*ngauss+j + (dim-firstdim)*(psize*ngauss);
		  pflmem[index] = i*ngauss*nbdim + j*nbdim+dim;
		  pfldsk[index] = dim*count[0] + (pfltab[i]-1)*ngauss+j;	     
		}
	    }
	    
	    if ( (ret = H5Sselect_elements(memspace,H5S_SELECT_SET, pcount[0], (const hssize_t **) pflmem ) ) <0) 
	      return -1; 
	    
	    if ( (ret = H5Sselect_elements(dataspace,H5S_SELECT_SET, pcount[0], (const hssize_t **) pfldsk ) ) <0) 
	      return -1; 
	     
	    break;
	  
	  default :
	    return -1; 
	  }

	if ((ret = H5Dwrite(dataset,type_hdf,memspace,dataspace,H5P_DEFAULT, val)) < 0)
	  return -1;
	
	free(pflmem);
	free(pfldsk);
      }
      
      
      break;
      
    case MED_NO_INTERLACE :

      /*Initialisation des indices de boucle du traitement de l'entrelacement en fonction de la dimension fixee*/

      count[0] = (*size)/nbdim;

      if ( psize == MED_NOPF ) {  
	
	if ( fixdim != MED_ALL) 
	  start_data[0] = (fixdim-1)*count[0];
	else {
	  count[0] = *size;
	  start_data[0] =  0;
	};
	
	if ( (ret = H5Sselect_hyperslab (dataspace, H5S_SELECT_SET, start_data, NULL, 
					 count, NULL)) <0)
	  return -1; 
	
	if ((ret = H5Dwrite(dataset,type_hdf,dataspace,dataspace,
			    H5P_DEFAULT, val)) < 0)
	  return -1;
	
      } else {

	if ( fixdim != MED_ALL) 
	  { 
	    firstdim = fixdim-1;
	    lastdim  = fixdim;
	    dimutil  = 1;
	  } else	{
	    firstdim = 0;
	    lastdim  = nbdim;
	    dimutil  = nbdim; 
	  }
	
	pflsize [0] = psize*ngauss*nbdim;
  	pcount  [0] = psize*ngauss*dimutil; /* nom pas très coherent avec count !!! A revoir */	
	pfldsk     = (med_ssize *) malloc(sizeof(med_ssize)*pcount[0]);

	switch(pflmod)
	  { /*switch plfmod pour NO_INTERLACE */
	  case MED_GLOBALE :
	    
	    for (dim=firstdim; dim < lastdim; dim++) {
	      
	      for (i=0; i < psize; i++)              /* i balaye le nbre d'élements du profil                */
		for (j=0; j < ngauss; j++) { 
		  index = i*ngauss+j + (dim-firstdim)*(psize*ngauss);
		  pfldsk[index] = dim*count[0]+(pfltab[i]-1)*ngauss+j;	    
		}
	    }
	    
	    if ( (ret = H5Sselect_elements(dataspace,H5S_SELECT_SET,pcount[0], (const hssize_t **) pfldsk ) ) <0) 
	      return -1;
	    
	    if ((ret = H5Dwrite(dataset,type_hdf,dataspace,dataspace,H5P_DEFAULT, val)) < 0)
	      return -1;
	    
	    break;
	    
	  case MED_COMPACT :
	    
	    /* Creation d'un data space mémoire de dimension 1, de la longeur du profil          */
	    /* La dimension utilisée est ici nbdim, même pour un profil compact on suppose       */
	    /*  que l'utilisateur a toutes les coordonées stockées, même si il en demande qu'une */ 

	    if ( (memspace = H5Screate_simple (1, pflsize, NULL)) <0)
	      return -1;

	    pflmem     = (med_ssize *) malloc (sizeof(med_ssize)*pcount[0]);
	    
	    /* Le profil COMPACT est contigüe, mais il est possible que l'on selectionne uniquemenent une dimension*/
	    
	    for (dim=firstdim; dim < lastdim; dim++) {
	      
	      for (i=0; i < psize; i++)              /* i balaye le nbre d'élements du profil                */
		for (j=0; j < ngauss; j++) {
		  index = i*ngauss+j + (dim-firstdim)*(psize*ngauss);
	          pflmem[index] = dim*(psize*ngauss) + (pfltab[i]-1)*ngauss+j;
		  pfldsk[index] = dim*count[0]  + (pfltab[i]-1)*ngauss+j;	    
		}
	    }
	     
	    if ( (ret = H5Sselect_elements(memspace ,H5S_SELECT_SET,pcount[0], (const hssize_t **) pflmem ) ) <0) 
	      return -1; 
	      
	    if ( (ret = H5Sselect_elements(dataspace,H5S_SELECT_SET,pcount[0], (const hssize_t **) pfldsk ) ) <0) 
	      return -1;
	   
	    if ((ret = H5Dwrite(dataset,type_hdf,memspace,dataspace,H5P_DEFAULT, val)) < 0)
	      return -1;

	    free(pflmem);
	    
	    break;
	    
	  default :
	    return -1;	    
	    
	  }
   
	free(pfldsk);
	
      };

      break;
      
    default :
      return -1;
    }
  
  
  if (memspace) 
    if ((ret = H5Sclose(memspace)) < 0)
      return -1;
  
  if ((ret = H5Sclose(dataspace)) < 0)
    return -1;
  
  if ((ret = H5Dclose(dataset)) < 0)
    return -1;      

#if defined(PCLINUX)
  if (type == MED_INT32)
      if ((H5Tconvert(H5T_STD_I32BE,H5T_NATIVE_INT,(hsize_t)*size,(void *)val,NULL,NULL)) < 0) 
	  return -1;
#endif 
  
  return 0;
}

}
