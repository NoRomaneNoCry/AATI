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
  IplImage* img = NULL, fil, seuil, affin;
  const char* src_path = NULL;
  const char* dst_path = NULL;

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
  std::cout<<"constructeur"<<std::endl;

  filtre f = filtrePrewitt(*img);


  //cvNamedWindow ("seuil", CV_WINDOW_AUTOSIZE);
  //cvShowImage ("avant", img);
  //Create trackbar to change contrast
  fil = f.filtreMultidirectionnel();
  std::cout<<"filtre"<<std::endl;
  cvNamedWindow ("filtre", CV_WINDOW_AUTOSIZE);
  //cvNamedWindow ("controle", CV_WINDOW_AUTOSIZE);
  cvNamedWindow ("seuil", CV_WINDOW_AUTOSIZE);
 // cvNamedWindow ("affinage", CV_WINDOW_AUTOSIZE);
  cvShowImage ("filtre", &fil);
  //int Seuillage = 50;
  //cvCreateTrackbar("Seuillage", "controle", &Seuillage, 255);
  //seuil = cvCreateImage (cvGetSize (&fil), IPL_DEPTH_8U, 1);
  seuillage s = seuillage(fil);
  //seuil = s.seuilHysteresis(44,60);
 // affin = s.affinage(f);
  seuil = s.seuilHysteresis(44,60);
  cvShowImage("seuil", &seuil);
  affin = s.affinage(f);
  cvShowImage("affinage", &affin);

  cvWaitKey(0);

  if (dst_path && !cvSaveImage (dst_path, &affin, NULL))
  {
    fprintf (stderr, "couldn't write image to file: %s\n", dst_path);
  }
  
  cvReleaseImage(&img);
  //cvReleaseImage(&fil);
 // cvReleaseImage(&seuil);
  return EXIT_SUCCESS;
}
