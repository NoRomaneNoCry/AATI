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
  IplImage* img = NULL, filb, film, seuil, affin;
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

  filtre f = filtrePrewitt(*img);

  filb = f.filtreBidirectionnel();
  cvNamedWindow ("filtre bidirectionnel", CV_WINDOW_AUTOSIZE);
  cvShowImage ("filtre bidirectionnel", &filb);

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
 // cvReleaseImage(&seuil);
  return EXIT_SUCCESS;
}
