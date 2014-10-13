#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <opencv/highgui.h>

#include "filtre.h"
#include "filtrePrewitt.h"



int main (int argc, char* argv[])
{
  IplImage* img = NULL, *fil = NULL; 
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
  fil = cvCreateImage (cvGetSize (img), IPL_DEPTH_8U, 1);
  f.appliqueFiltre(img, fil);
  cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
  cvShowImage (window_title, fil);
  cvWaitKey(0);

  if (dst_path && !cvSaveImage (dst_path, fil, NULL))
  {
    fprintf (stderr, "couldn't write image to file: %s\n", dst_path);
  }
  cvReleaseImage(&img);
  cvReleaseImage(&fil);

  return EXIT_SUCCESS;
}
