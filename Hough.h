#ifndef HOUGH_H
#define HOUGH_H

#include <opencv/highgui.h>
#include <vector>

class Hough
{
    public:
        Hough(const IplImage &img);
        ~Hough();
        void vote(int x,int y);
        std::vector<double> gagnant();
        std::pair<double, double> val(const int i, const int j);    
        std::pair<double, double> rhoThetaToAb(const double rho, const double theta);
        void AfficheAccumulateur(IplImage &res);

    private:
        int largeur,hauteur;
        int maxIndexTheta, maxIndexRho;
        double maxRho;
        IplImage img;
        std::vector<std::vector<int> > accumulateur;

};

#endif // HOUGH_H
