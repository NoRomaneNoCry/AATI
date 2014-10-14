#include "filtreLaplacien.h"

#include <iostream>

filtreLaplacien::filtreLaplacien()
{
	Gx.resize(3);
	Gy.resize(3);
	for (int i = 0; i < getNbLigne(); ++i)
	{
		Gx[i].resize(3,1);
		Gy[i].resize(3,1);
	}
	Gx[1][1] = Gy[1][1] = -4; 
	Gx[0][0] = Gx[0][2] =Gx[2][0] = Gx[2][2] = 0;
	Gy[0][0] = Gy[0][2] =Gy[2][0] = Gy[2][2] = 0;
}

filtreLaplacien::~filtreLaplacien()
{

}
