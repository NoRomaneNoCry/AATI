#include "filtrePrewitt.h"

#include <iostream>

filtrePrewitt::filtrePrewitt(const IplImage& image): filtre(image)
{	
	getGV(0,0) = getGV(0,1) = getGV(0,2) = 1; 
	getGV(2,0) = getGV(2,1) = getGV(2,2) = -1; 
	for (int i = 0; i < getNbLigne(); ++i)
	{
		for (int j = 0; j < getNbColonne(); ++j)
		{
			getGH(i,j) = getGV(j,i);
		}
	}
	getDiag(0,0) = getDiag(1,0) = getDiag(0,1) = 1;
	getDiag(2,2) = getDiag(2,1) = getDiag(1,2) = -1;
}

filtrePrewitt::~filtrePrewitt()
{

}
