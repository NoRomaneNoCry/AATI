/*
CHEMIER Aurélien
LHOMME Romane
*/

#ifndef FILTRE_PREWITT_H
#define FILTRE_PREWITT_H

#include "filtre.h"

class filtrePrewitt : public filtre
{

public:
	filtrePrewitt(const IplImage& image);
	~filtrePrewitt();
};

#endif
