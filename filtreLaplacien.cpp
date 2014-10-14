#include "filtreLaplacien.h"

#include <stdio.h>
#include <stdlib.h>

filtreLaplacien::filtreLaplacien()
{
	Gx.resize(3);
	Gy.resize(3);
	for (int i = 0; i < getNbLigne(); ++i)
	{
		Gx[i].resize(3,1);
		Gy[i].resize(3,1);
	}
	Gx[1][1] = -8; 
	Gy[1][1] = -8; 

	for (int i = 0; i < getNbLigne(); ++i)
	{

		printf("%d %d %d\n", Gy[i][0],Gy[i][1],Gy[i][2]);
	}
	for (int i = 0; i < getNbLigne(); ++i)
	{

		printf("%d %d %d\n", Gx[i][0],Gx[i][1],Gx[i][2]);
	}
}

filtreLaplacien::~filtreLaplacien()
{

}
