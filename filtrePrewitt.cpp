#include "filtrePrewitt.h"

#include <iostream>

filtrePrewitt::filtrePrewitt(): filtre(3,3)
{
	GV[0][0] = GV[0][1] = GV[0][2] = 1; 
	GV[2][0] = GV[2][1] = GV[2][2] = -1; 
	
	for (int i = 0; i < getNbLigne(); ++i)
	{
		for (int j = 0; j < getNbColonne(); ++j)
		{
			GH[i][j] = GV[j][i];
		}
	}
	Diag[0][0] = Diag[1][0] = Diag[0][1] = 1;
	Diag[2][2] = Diag[2][1] = Diag[1][2] = -1;
}

filtrePrewitt::~filtrePrewitt()
{

}
