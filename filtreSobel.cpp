#include "filtreSobel.h"

#include <stdio.h>
#include <stdlib.h>

filtreSobel::filtreSobel(): filtre(3,3)
{
	GV[0][0] = GV[2][0] = 1; 
	GV[1][0] = 2;
	GV[1][2] = -2;
	GV[0][2] = GV[2][2] = -1;

	for (int i = 0; i < GV.size(); ++i)
	{
		for (int j = 0; j < GV[i].size(); ++j)
		{
			GH[i][j] = GV[j][i];
		}
	}
	Diag[0][0] = 2;
	Diag[1][0] = Diag[0][1] = 1;
	Diag[2][2] = -2;
	Diag[2][1] = Diag[1][2] = -1;
}

filtreSobel::~filtreSobel()
{

}

