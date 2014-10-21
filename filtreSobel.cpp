#include "filtreSobel.h"

#include <stdio.h>
#include <stdlib.h>

filtreSobel::filtreSobel(): filtre(3,3)
{
	
	Gx[0][0] = Gx[2][0] = 1; 
	Gx[1][0] = 2;
	Gx[1][2] = -2;
	Gx[0][2] = Gx[2][2] = -1;

	for (int i = 0; i < Gx.size(); ++i)
	{
		for (int j = 0; j < Gx[i].size(); ++j)
		{
			Gy[i][j] = Gx[j][i];
		}
	}


}

filtreSobel::~filtreSobel()
{

}

