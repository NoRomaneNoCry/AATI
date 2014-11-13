/*
CHEMIER Aurélien
LHOMME Romane
*/

#include "filtreSobel.h"

filtreSobel::filtreSobel(const IplImage& image): filtre(image)
{
	getGV(0,0) = getGV(2,0) = 1; 
	getGV(1,0) = 2;
	getGV(1,2) = -2;
	getGV(0,2) = getGV(2,2) = -1;

	for (int i = 0; i < getNbLigne(); ++i) {
		for (int j = 0; j < getNbColonne(); ++j) {
			getGH(i,j) = getGV(j,i);
		}
	}
	getDiag(0,0) = 2;
	getDiag(1,0) = getDiag(0,1) = 1;
	getDiag(2,2) = -2;
	getDiag(2,1) = getDiag(1,2) = -1;
}

filtreSobel::~filtreSobel()
{

}

