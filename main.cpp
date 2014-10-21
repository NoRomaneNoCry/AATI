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


int main (int argc, char* argv[])
{
  IplImage* img = NULL, *fil = NULL, *seuil = NULL;
  const char* src_path = NULL;
  const char* dst_path = NULL;
  const char* window_title = "filtre";
  filtre f = filtrePrewitt();

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
  cvNamedWindow ("avant", CV_WINDOW_AUTOSIZE);
  cvShowImage ("avant", img);
  //Create trackbar to change contrast
  fil = cvCreateImage (cvGetSize (img), IPL_DEPTH_8U, 1);
  f.appliqueFiltre(*img, *fil);
  //cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
  //cvNamedWindow ("controle", CV_WINDOW_AUTOSIZE);
  // cvNamedWindow ("seuil", CV_WINDOW_AUTOSIZE);
  cvShowImage (window_title, fil);
  //int Seuillage = 50;
  //cvCreateTrackbar("Seuillage", "controle", &Seuillage, 255);
  seuil = cvCreateImage (cvGetSize (fil), IPL_DEPTH_8U, 1);
  f.seuilHysteresis(*fil,*seuil,20,150);
 //f.seuilFixe(*fil,*seuil,50);

  cvShowImage ("seuil", seuil);

  cvWaitKey(0);

  if (dst_path && !cvSaveImage (dst_path, fil, NULL))
  {
    fprintf (stderr, "couldn't write image to file: %s\n", dst_path);
  }
  cvReleaseImage(&img);
  cvReleaseImage(&fil);
  return EXIT_SUCCESS;
}
