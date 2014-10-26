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

	IplImage seuilFixe(const unsigned int seuil);
    IplImage seuilGlobal();
    IplImage seuilLocal();
    IplImage seuilHysteresis(const unsigned int SeuilBas, const unsigned int SeuilHaut);
    IplImage seuillageExtractionMaximasLocaux(const filtre &f);
};

#endif
