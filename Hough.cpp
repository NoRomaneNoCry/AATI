#include "Hough.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

Hough::Hough(const IplImage & image): largeur(image.width), hauteur(image.height), img(image)
{
    maxRho = (double)sqrt(pow((double)largeur,2) + pow((double)hauteur,2));
    //maxRho = (double)sqrt(2)*std::max(largeur, hauteur)/2;
    maxIndexTheta = 360;
    maxIndexRho = (int)(1 + maxRho);
    accumulateur.resize(maxIndexTheta);
    for (int i = 0; i < maxIndexTheta; ++i)
    {
        accumulateur[i].resize(maxIndexRho);
    }
}

Hough::~Hough()
{}



void Hough::vote(int x,int y)
{
      x -= largeur/2;
    y -= hauteur/2;

    for(int iTheta = 0; iTheta < maxIndexTheta; iTheta++)
    {
        double theta = ((double)iTheta/maxIndexTheta)*M_PI;

        // calcul de rho
        double rho = x*cos(theta) + y*sin(theta);

        // rho -> index
        int iRho   = (int) (0.5 + (rho/maxRho+ 0.5)*maxIndexRho );

        // increment accumulator
        accumulateur[iTheta][iRho]++;
    }
}

std::vector<double> Hough::gagnant()
{
    std::vector<double> retour;

    // parsing the accumulators for max accumulator
    double max=0, fenRho=0, fenTheta=0;

    for(int iRho = 0; iRho < maxIndexRho; iRho++)
    {
            for(int iTheta = 0; iTheta < maxIndexTheta; iTheta++)
            {
                if (accumulateur[iTheta][iRho] < max)
                {
                    max = accumulateur[iRho][iTheta];
                    fenRho = iRho;
                    fenTheta = iTheta;
                    double rho   = ((double)fenRho  / maxIndexRho - 0.5) * maxRho;
                    double theta = ((double)fenTheta/maxIndexTheta) * M_PI;

                    retour.push_back(rho);
                    retour.push_back(theta);
                }
            }
    }

    

    return retour;
}


std::pair<double, double> Hough::val(const int iRho, const int iTheta)
{
    std::pair<double, double> retour;

    double rho   = ((double)iRho  / maxIndexRho - 0.5) * maxRho;
    double theta = ((double)iTheta / maxIndexTheta) * M_PI;

    retour.first = rho;
    retour.second = theta;
      
    return retour;
}

std::pair<double, double> Hough::rhoThetaToAb(const double rho, const double theta)
{
    std::pair<double, double> retour;

    double a = 0, b = 0;

    if(sin(theta)!=0)
    {
        a = -cos(theta) / sin(theta);
        b = rho / sin(theta) + (largeur/2) - a * (hauteur/2);
    }
    else
    {
        a = 99999;
        b = 0;
    }

    retour.first = a;
    retour.second = b;

    return retour;
}


void Hough::AfficheAccumulateur(IplImage &res)
{
   res = *cvCreateImage( cvSize(maxIndexRho, maxIndexTheta ), IPL_DEPTH_8U, 1 );
    //IplImage res = *cvCreateImage( cvSize(largeur, hauteur), IPL_DEPTH_8U, 1 );

  for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
           if(cvGet2D (&img, i, j).val[0] == 255)
                vote(i,j);
        }
    }
    //std::vector<double> g = gagnant();
    int max = 0;
    for(int iRho = 0; iRho < maxIndexRho; iRho++) {
        for(int iTheta = 0; iTheta < maxIndexTheta; iTheta++) {
            if (accumulateur[iTheta][iRho] > max)
                max = accumulateur[iTheta][iRho];
        }
    }

    std::pair<double, double> rhoTheta;
    std::pair<double, double> xy;
    CvScalar grad;

    for(int i = 0; i < maxIndexTheta; i++) {
        for(int j = 0; j < maxIndexRho; ++j) {
           // rhoTheta = val(j, i);
           // xy = rhoThetaToAb(rhoTheta.second, rhoTheta.first);
            //if(xy.first != 99999)
                //cvGet2D(&res, i, j).val[0] = 255;

                grad.val[0] = accumulateur[i][j];
                cvSet2D(&res,i,j,grad);
            }
    }

    std::cout<<"fin"<<std::endl;
    //return res;
}
