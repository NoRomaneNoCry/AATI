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
        std::vector<double> rhoThetaToAb(const double rho, const double theta);
        IplImage AfficheAccumulateur();

    private:
        int largeur,hauteur;
        int maxIndexTheta, maxIndexRho;
        double maxRho;
        IplImage img;
        std::vector<std::vector<int> > accumulateur;

};

#endif // HOUGH_H
