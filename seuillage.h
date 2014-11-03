#ifndef SEUILLAGE_H
#define SEUILLAGE_H 

#include <opencv/highgui.h>
#include <iostream>
#include <assert.h>

#include "filtre.h"

class seuillage
{
private:
	IplImage img;
	IplImage resSeuillage;
public:
	seuillage();
	seuillage(IplImage image);
	~seuillage();

	/*calcul le seuillage fixe de l'image*/
	IplImage seuilFixe(unsigned int seuil);
	/*calcul le seuillage global de l'image*/
    IplImage seuilGlobal();
    /*calcul le seuillage local de l'image*/
    IplImage seuilLocal();
    /*calcul le seuillage hysteresis de l'image*/
    IplImage seuilHysteresis(unsigned int SeuilBas, unsigned int SeuilHaut);
    /*affine le seuillage de l'image*/
    IplImage affinage(const filtre& f);
};

#endif
