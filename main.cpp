#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <opencv/highgui.h>
#include <algorithm>

#include "filtre.h"
#include "filtrePrewitt.h"
#include "filtreSobel.h"
#include "filtreKirsch.h"
#include "seuillage.h"


int main (int argc, char* argv[])
{
  IplImage* img = NULL/*, filb, film, seuil, affin, filbc;*/; 
  IplImage imgfil;
  IplImage imgSeuil;
  IplImage imgAffine;
  filtre fil;
  const char* src_path = NULL;
  const char* dst_path = NULL;
  char * aff = NULL;
  bool ok = false;
  int f, m, s, sf, sh = -1,sb = -1;

  if (argc < 2)
  {
    fprintf (stderr, "usage: %s IMAGE [SAVE_PATH]\n", argv[0]);
    return EXIT_FAILURE;
  }

  src_path = argv[1];
  // optionnel: sauvegarde du résultat
  // si un second chemin est passé au programme
  if (argc > 2)
    dst_path = argv[2];
  if (!(img = cvLoadImage (src_path, CV_LOAD_IMAGE_GRAYSCALE)))
  {
    fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  std::cout<<"Choisissez le masque à appliquer"<<std::endl;
  std::cout<<"1 : Prewitt"<<std::endl;
  std::cout<<"2 : Sobel"<<std::endl;
  std::cout<<"3 : Kirsh"<<std::endl;

  while(!ok)
  {
    std::cin>>m;
    switch(m)
    {
      case 1:
        fil = filtrePrewitt(*img);
        ok = true;
        break;
      case 2:
        fil = filtreSobel(*img);
        ok = true;
        break;
      case 3:
        fil = filtreKirsch(*img);
        ok = true;
        break;
      default:
        std::cout<<"valeur non valide "<<std::endl;
    }
  }
  
  std::cout<<"Choisissez le filtre à appliquer"<<std::endl;
  std::cout<<"1 : horizontal"<<std::endl;
  std::cout<<"2 : vertical"<<std::endl;
  std::cout<<"3 : diagonal"<<std::endl;
  std::cout<<"4 : bidirectionnel"<<std::endl;
  std::cout<<"5 : bidirectionnel en couleur"<<std::endl;
  std::cout<<"6 : multidirectionnel"<<std::endl;
  std::cout<<"7 : multidirectionnel en couleur"<<std::endl;
  ok = false;

  while(!ok)
  {
    std::cin>>f;
    switch(f)
    {
      case 1:
        imgfil = fil.filtreHorizontal();
        ok = true;
        break;
      case 2:
        imgfil = fil.filtreVertical();
        ok = true;
        break;
      case 3:
        imgfil = fil.filtreDiagonalD();
        ok = true;
        break;
      case 4:
        imgfil = fil.filtreBidirectionnel();
        ok = true;
        break;
      case 5:
        imgfil = fil.filtreBidirectionnelCouleur();
        ok = true;
        break;
      case 6:
        imgfil = fil.filtreMultidirectionnel();
        ok = true;
        break;
      case 7:
        imgfil = fil.filtreMultidirectionnelCouleur();
        ok = true;
        break;
      default:
        std::cout<<"valeur non valide "<<std::endl;
    }
  }

  std::cout<<"Choisissez le seuil"<<std::endl;
  std::cout<<"1 : fixe"<<std::endl;
  std::cout<<"2 : gobal"<<std::endl;
  std::cout<<"3 : local"<<std::endl;
  std::cout<<"4 : Hysteresis"<<std::endl;
  ok = false;
  seuillage seuil = seuillage(imgfil);

  while(!ok)
  {
    std::cin>>s;
    switch(s)
    {
      case 1:
        while(sf < 0 || sf >  255)
        {
          std::cout<<"Donnez la valeur du seuil"<<std::endl;
          std::cin>>sf;
        }
        imgSeuil = seuil.seuilFixe(sf);

        ok = true;
        break;
      case 2:
         imgSeuil = seuil.seuilGlobal();
        ok = true;
        break;
      case 3:
        imgSeuil = seuil.seuilLocal();
        ok = true;
        break;
      case 4:
        while(sb < 0 || sb > 255)
        {
          std::cout<<"Donnez la valeur du seuil bas"<<std::endl;
          std::cin>>sb;
        }
        while(sh < 0 || sh > 255 || sh < sb)
        {
          std::cout<<"Donnez la valeur du seuil haut"<<std::endl;
          std::cin>>sh;
        }
        imgSeuil = seuil.seuilHysteresis(sb,sh);
        ok = true;
        break;
      default:
        std::cout<<"valeur non valide "<<std::endl;
    }
  }

  std::cout<<"affiner les contour ? (O/N)"<<std::endl;  

  while(aff != NULL && strcmp(aff,"O") != 0 && strcmp(aff,"o") != 0 
    && strcmp(aff,"N") != 0 && strcmp(aff,"n") != 0)
  {
    std::cin>>aff;
    if(strcmp(aff,"O") == 0 || strcmp(aff,"o") == 0)
    {
      imgAffine = seuil.affinage(fil);
    }
  }


  cvNamedWindow ("image filtrée", CV_WINDOW_AUTOSIZE);
  cvShowImage ("image filtrée", &imgfil);
  cvWaitKey(0);
  cvNamedWindow ("image seuillée", CV_WINDOW_AUTOSIZE);
  cvShowImage ("image seuillée", &imgSeuil);
  cvWaitKey(0);


/*
  filtre f = filtrePrewitt(*img);
  filtre fc = filtrePrewitt(*img);
  //cvReleaseImage(&film);

  filb = f.filtreBidirectionnel();
  cvNamedWindow ("filtre bidirectionnel", CV_WINDOW_AUTOSIZE);
  cvShowImage ("filtre bidirectionnel", &filb);


  filbc = fc.filtreBidirectionnelCouleur();
  cvNamedWindow ("filtre bidirectionnel Couleur", CV_WINDOW_AUTOSIZE);  
  cvShowImage ("filtre bidirectionnel Couleur", &filbc);

  film = f.filtreMultidirectionnel();
  cvNamedWindow ("filtre multidirectionnel", CV_WINDOW_AUTOSIZE);
  cvShowImage ("filtre multidirectionnel", &film);

  cvNamedWindow ("seuillage Hysteresis", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("affinage", CV_WINDOW_AUTOSIZE);

  seuillage s = seuillage(film);
  seuil = s.seuilHysteresis(44,60);
  affin = s.affinage(f);
  cvShowImage("seuillage Hysteresis", &seuil);
  cvShowImage("affinage", &affin);

  cvWaitKey(0);

  if (dst_path && !cvSaveImage (dst_path, &film, NULL))
  {
    fprintf (stderr, "couldn't write image to file: %s\n", dst_path);
  }
  
  cvReleaseImage(&img);
  //cvReleaseImage(&fil);
 // cvReleaseImage(&seuil);*/
  return EXIT_SUCCESS;



}
