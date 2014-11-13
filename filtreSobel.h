/*
CHEMIER Aurélien
LHOMME Romane
*/

#ifndef FILTRE_SOBEL_H
#define FILTRE_SOBEL_H

#include "filtre.h"

class filtreSobel : public filtre
{

public:
	filtreSobel(const IplImage& image);
	~filtreSobel();
};

#endif
