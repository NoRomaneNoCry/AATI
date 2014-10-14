#include <assert.h>
#include <math.h>

#include "filtre.h"

filtre::filtre():nbLigne(3), nbColonne(3)
{
	filtre(3,3);
}

filtre::filtre(const unsigned int ligne,const unsigned int colonne): nbLigne(ligne), nbColonne(colonne)
{
	Gx.resize(nbLigne);
	Gy.resize(nbLigne);
	for (int i = 0; i < nbLigne; ++i)
	{
		Gx[i].resize(nbColonne);
		Gy[i].resize(nbColonne);
	}
}

filtre::filtre(const filtre &origin): nbLigne(origin.getNbLigne()), 
									  nbColonne(origin.getNbColonne())
{
	Gx.resize(nbLigne);
	Gy.resize(nbLigne);
	for (int i = 0; i < nbLigne; ++i)
	{
		Gx[i].resize(nbColonne);
		Gy[i].resize(nbColonne);
		for (int j = 0; j < nbColonne; ++j)
		{
			Gx[i][j] = origin.getGx(i,j);
			Gy[i][j] = origin.getGy(i,j);
		}
	}
}

filtre::~filtre()
{

}

unsigned int& filtre::getNbLigne()
{
	return nbLigne;
}

unsigned int& filtre::getNbColonne()
{
	return nbColonne;
}

unsigned int filtre::getNbLigne() const
{
	return nbLigne;
}

unsigned int filtre::getNbColonne() const
{
	return nbColonne;
}

int& filtre::getGy(const unsigned int x, const unsigned int y)
{
	return Gy[x][y];
}

int filtre::getGy(const unsigned int x, const unsigned int y) const
{
	return Gy[x][y];
}

int& filtre::getGx(const unsigned int x, const unsigned int y)
{
	return Gx[x][y];
}

int filtre::getGx(const unsigned int x, const unsigned int y) const
{
	return Gx[x][y];
}

filtre& filtre::operator=(filtre f)
{
	filtre(f.getNbLigne(),f.getNbColonne());
	for (int i = 0; i < getNbLigne(); ++i)
	{
		for (int j = 0; j < getNbColonne(); ++j)
		{
			Gx[i][j] = f.getGx(i,j);
			Gy[i][j] = f.getGy(i,j);
		}
	}
	return *this;
}

void filtre::appliqueFiltre(IplImage* img, IplImage *res)
{
	int x,y,i,j;
	uchar *p;

	uchar u;
	int Sx,Sy;

	assert (img->depth == IPL_DEPTH_8U && img->nChannels == 1);

	for (y = 1; y < img->height-1; ++y)
	{
		for (x = 1; x < img->width-1; ++x)
		{
		Sx = 0;
		Sy = 0;
		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				u = *cvPtr2D(img, y-1+j, x-1+i, NULL); 
				Sx += u * Gx[j][i];				
				Sy += u * Gy[j][i];
			}
		}
		p = cvPtr2D (res, y, x, NULL);
		*p = sqrt(Sy*Sy +Sx*Sx);
		*p %= 255;
		}
	}
}

void filtre::appliqueFiltreCouleur(IplImage* img, IplImage *res)
{
	int x,y,i,j;
	uchar *p;

	uchar *u;
	int Bx,By;
	int Rx,Ry;
	int Vx,Vy;

	assert (img->depth == IPL_DEPTH_8U && img->nChannels == 3);

	for (y = 1; y < img->height-1; ++y)
	{
		for (x = 1; x < img->width-1; ++x)
		{
		Bx = 0;By = 0;
		Vx = 0;Vy = 0;
		Rx = 0;Ry = 0;

		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				u = cvPtr2D(img, y-1+j, x-1+i, NULL); 
				Bx += *u * Gx[j][i];				
				By += *u * Gy[j][i];
				Vx += *(u+1) * Gx[j][i];				
				Vy += *(u+1) * Gy[j][i];
				Rx += *(u+2) * Gx[j][i];				
				Ry += *(u+2) * Gy[j][i];
			}
		}
		p = cvPtr2D (res, y, x, NULL);
		*p = sqrt(By*By + Bx*Bx);
		*p %= 255;

		*(p+1) = sqrt(Vy*Vy + Vx*Vx);
		*(p+1) %= 255;
		*(p+2) = sqrt(Ry*Ry + Rx*Rx);
		*(p+2) %= 255;
		}
	}
}
