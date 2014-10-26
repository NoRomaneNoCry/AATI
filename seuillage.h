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

	IplImage seuilFixe(unsigned int seuil);
    IplImage seuilGlobal();
    IplImage seuilLocal();
    IplImage seuilHysteresis(unsigned int SeuilBas, unsigned int SeuilHaut);
    IplImage seuillageExtractionMaximasLocaux(const filtre &f);
    IplImage affinage(const filtre& f);
};

#endif
