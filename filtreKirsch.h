#ifndef FILTRE_KIRSCH_H
#define FILTRE_KIRSCH_H

#include <vector>

#include "filtre.h"

class filtreKirsch : public filtre
{

public:
	filtreKirsch(const IplImage& image);
	~filtreKirsch();
};

#endif
