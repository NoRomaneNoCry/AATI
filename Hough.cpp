/*
CHEMIER Aurélien
LHOMME Romane
*/

#include "Hough.h"

Hough::Hough(const IplImage & image): largeur(image.width), hauteur(image.height), img(image)
{
    maxRho = (double)sqrt(pow((double)largeur,2) + pow((double)hauteur,2));
    //maxRho = (double)sqrt(2)*std::max(largeur, hauteur)/2;
    maxIndexTheta = 360;
    maxIndexRho = (int)(1 + maxRho);
    accumulateur.resize(maxIndexTheta);
    for (int i = 0; i < maxIndexTheta; ++i) {
        accumulateur[i].resize(maxIndexRho);
    }
    remplirAccumulateur();
}

Hough::~Hough()
{}

void Hough::calcul(int x, int y)
{
    x -= largeur/2;
    y -= hauteur/2;

    for(int iTheta = 0; iTheta < maxIndexTheta; iTheta++) {
        double theta = ((double)iTheta/maxIndexTheta)*M_PI;

        double rho = x*cos(theta) + y*sin(theta);

        int iRho   = (int) (0.5 + (rho/maxRho+ 0.5)*maxIndexRho );

        accumulateur[iTheta][iRho]++;
    }
}

std::vector<std::pair<double, double> > Hough::seuil(const int seuil)
{
    std::vector<std::pair<double, double> > retour;

    for(int iRho = 0; iRho < maxIndexRho; iRho++) {
        for(int iTheta = 0; iTheta < maxIndexTheta; iTheta++) {
            if (accumulateur[iTheta][iRho] > seuil)
                retour.push_back(val(iRho, iTheta));
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

void Hough::afficheAccumulateur(IplImage &res)
{
    res = *cvCreateImage( cvSize(maxIndexRho, maxIndexTheta ), IPL_DEPTH_8U, 1 );

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
            grad.val[0] = 255*accumulateur[i][j]/max;
            cvSet2D(&res,i,j,grad);
        }
    }
}

void Hough::afficheDroite(IplImage &res, const int seuil)
{
    res = *cvCreateImage( cvSize(img.width, img.height ), IPL_DEPTH_8U, 3);
    //std::vector<std::pair<double, double> > v = seuil(100);
    CvScalar blanc; 
    blanc.val[0] = blanc.val[1] = blanc.val[2] = 255;
    for (int x = 0; x < img.height; ++x) {
        for (int y = 0; y < img.width; ++y) {
            if(cvGet2D(&img, x, y).val[0] == 255)
                cvSet2D (&res, x, y, blanc);
        }
    }

    for(int theta=0; theta < maxIndexTheta; theta++)  
    {  
        for(int rho=0; rho < maxIndexRho; rho++)  
        {  
            if(accumulateur[theta][rho] >= seuil)  
            {  
                //maximum local (9x9)  
                int max = accumulateur[theta][rho];  
                for(int ly=-4;ly<=4;ly++)  
                {  
                    for(int lx=-4;lx<=4;lx++)  
                    {  
                        if( (ly+theta >= 0 && ly+theta < maxIndexTheta) && (lx+rho >= 0 && lx+rho <maxIndexRho) )  
                        {  
                            if( accumulateur[theta+ly][rho+lx] > max )  
                            {  
                                max = accumulateur[theta+ly][rho+lx];  
                                ly = lx = 5;  
                            }  
                        }  
                    }  
                }  
                if(max < accumulateur[theta][rho])  
                {
                    int x1, y1, x2, y2;  
                    x1 = y1 = x2 = y2 = 0;  

                    if(theta >= 45 && theta <= 135)  
                    {  
                        //y = (rho - x cos(theta)) / sin(theta)  
                        x1 = 0;  
                        y1 = rho - (x1 + (largeur/2)) * cos(((double)theta / maxIndexTheta) * M_PI) / sin(((double)theta / maxIndexTheta) * M_PI) + (hauteur / 2);
                        x2 = largeur - 0;  
                        y2 = rho - (x2 + (largeur/2)) * cos(((double)theta / maxIndexTheta) * M_PI) / sin(((double)theta / maxIndexTheta) * M_PI) + (hauteur / 2);  
                    }  
                    else  
                    {  
                        //x = (rho - y sin(theta)) / cos(theta);  
                        y1 = 0;  
                        x1 = rho - (y1 + (hauteur/2)) * sin(((double)theta / maxIndexTheta) * M_PI) / cos(((double)theta / maxIndexTheta) * M_PI) + (largeur / 2);  
                        y2 = hauteur - 0;  
                        x2 = rho - (y2 + (hauteur/2)) * sin(((double)theta / maxIndexTheta) * M_PI) / cos(((double)theta / maxIndexTheta) * M_PI) + (largeur / 2);  
                    }  

                    CvPoint pt1, pt2;
                    pt1.x = cvRound(x1);
                    pt1.y = cvRound(y1);
                    pt2.x = cvRound(x2);
                    pt2.y = cvRound(y2);
                    cvLine( &res, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );
                }               
            }  
        }  
    }  
}

void Hough::remplirAccumulateur()
{
    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
           if(cvGet2D (&img, i, j).val[0] == 255)
                calcul(i,j);
        }
    }
}
