#include "filtreKirsch.h"

#include <stdio.h>
#include <stdlib.h>

filtreKirsch::filtreKirsch(): filtre(3,3)
{
	GV[0][0] = GV[0][1] = -3; 
	GV[2][0] = GV[2][1] = -3; 
	GV[1][0] = -3; 
	GV[1][2] = GV[0][2] = GV[2][2] = 5; 

	for (int i = 0; i < getNbLigne(); ++i)
	{
		for (int j = 0; j < getNbColonne(); ++j)
		{
			GH[i][j] = GV[j][i];
		}
	}

	Diag[0][0] = Diag[1][0] = Diag[0][1] = 5;
	Diag[2][2] = Diag[2][1] = Diag[1][2] = Diag[0][2] = Diag[2][0] = -3;
}

filtreKirsch::~filtreKirsch()
{

}
