/*
CHEMIER Aurélien
LHOMME Romane
*/

#ifndef HOUGH_H
#define HOUGH_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include <vector>

class Hough
{
private:
    int largeur,hauteur;
    int maxIndexTheta, maxIndexRho;
    double maxRho;
    IplImage img;
    std::vector<std::vector<int> > accumulateur;

public:
    /*constructeur*/
    Hough(const IplImage &img);
    ~Hough();

    void calcul(int x, int y);
    std::vector<std::pair<double, double> > seuil(const int seuil);
    std::pair<double, double> val(const int i, const int j);    
    std::pair<double, double> rhoThetaToAb(const double rho, const double theta);
    void afficheAccumulateur(IplImage &res);
    void afficheDroite(IplImage &res, const int seuil);
    void remplirAccumulateur();
};      

#endif // HOUGH_H
