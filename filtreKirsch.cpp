#include "filtreKirsch.h"

#include <stdio.h>
#include <stdlib.h>

filtreKirsch::filtreKirsch(const IplImage& image): filtre(image)
{
	getGV(0,0) = getGV(0,1) = -3; 
	getGV(2,0) = getGV(2,1) = -3; 
	getGV(1,0) = -3; 
	getGV(1,2) = getGV(0,2) = getGV(2,2) = 5; 

	for (int i = 0; i < getNbLigne(); ++i)
	{
		for (int j = 0; j < getNbColonne(); ++j)
		{
			getGH(i,j) = getGV(j,i);
		}
	}

	getDiag(0,0) = getDiag(1,0) = getDiag(0,1) = 5;
	getDiag(2,2) = getDiag(2,1) = getDiag(1,2) = getDiag(0,2) = getDiag(2,0) = -3;
}

filtreKirsch::~filtreKirsch()
{

}
