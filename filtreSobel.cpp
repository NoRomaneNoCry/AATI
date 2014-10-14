#include "filtreSobel.h"

#include <stdio.h>
#include <stdlib.h>

filtreSobel::filtreSobel()
{
	Gx.resize(3);
	Gy.resize(3);
	for (int i = 0; i < getNbLigne(); ++i)
	{
		Gx[i].resize(3);
		Gy[i].resize(3);
	}
	Gx[0][0] = Gx[2][0] = 1; 
	Gx[1][0] = 2; 
	Gx[1][2] = -2; 
	Gx[0][2] = Gx[2][2] = -1; 

	for (int i = 0; i < getNbLigne(); ++i)
	{
		for (int j = 0; j < getNbColonne(); ++j)
		{
			Gy[i][j] = Gx[j][i];
		}
	}


}

filtreSobel::~filtreSobel()
{

}

