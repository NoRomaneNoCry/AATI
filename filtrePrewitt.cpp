#include "filtrePrewitt.h"

#include <stdio.h>
#include <stdlib.h>

filtrePrewitt::filtrePrewitt()
{
	Gx.resize(3);
	Gy.resize(3);
	for (int i = 0; i < getNbLigne(); ++i)
	{
		Gx[i].resize(3,-1+i);
		Gy[i].resize(3);
		for (int j = 0; j < getNbColonne(); ++j)
		{
			Gy[i][j] = -1 +j;
		}
	}
}

filtrePrewitt::~filtrePrewitt()
{

}
