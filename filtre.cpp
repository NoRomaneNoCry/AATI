#include <assert.h>
#include <math.h>

#include "filtre.h"
#include <iostream>


#define PI 3.14159265

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

void filtre::appliqueFiltre(IplImage& img, IplImage &res)
{
	int x,y,i,j;
	uchar *p;
	int Sx,Sy;
	int grad;
	double ampli = 0.0;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 3);

	for (y = 1; y < img.height-1; ++y)
	{
		for (x = 1; x < img.width-1; ++x)
		{
			Sx = 0;
			Sy = 0;
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					p = cvPtr2D(&img, y-1+j, x-1+i, NULL); 
					Sx += *p * Gx[j][i];				
					Sy += *p * Gy[j][i];
				}
			}
			
			if(Sx != 0)
			{
				ampli = atan((double)Sy/(double)Sx);
				
				//std::cout<<"ampli "<<ampli;
				//std::cout<<" Sy "<<Sy<<" Sx " <<Sx <<std::endl;
				p = cvPtr2D (&res, y, x, NULL);
				grad = sqrt(Sx*Sx + Sy*Sy);
				grad %= 255;

				if(ampli >= 0 && ampli < PI/4)
				{
					*p = grad;
				}
				else if(ampli >= PI/4)
				{
					*(p+1) = grad;
				}
				else if(ampli <= 0 && ampli > -PI/4)
				{
					*(p+2) = grad;	
				}
				else 
				{
					*p = *(p+1) = *(p+2) = grad;
				}
			}
		}
	}
}

void filtre::seuilFixe(const IplImage& img, IplImage& res, const unsigned int seuil)
{
	int x,y;
	uchar *image, *result;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 3);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 3);

	for (y = 1; y < img.height; ++y)
	{
		for (x = 1; x < img.width; ++x)
		{
			image = cvPtr2D (&img, y, x, NULL);
			result = cvPtr2D (&res, y, x, NULL);
			if(*image < seuil)
				*result = 0;
			else *result = 255;
			if(*(image+1) < seuil)
				*(result+1) = 0;
			else *(result+1) = 255;
			if(*(image+2) < seuil)
				*(result+2) = 0;
			else *(result+2) = 255;
		}
	}
}

void filtre::seuilGlobal(const IplImage& img, IplImage& res)
{
	int x,y;
	unsigned int compteur = 0, somme = 0;
	double moyenne;
	uchar * u;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 3);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 3);

	for (y = 1; y < img.height; ++y)
	{
		for (x = 1; x < img.width; ++x)
		{
			u = cvPtr2D (&img, y, x, NULL);
			somme += *u;
			somme += *(u+1);
			somme += *(u+2);
			compteur +=3;
		}
	}

	moyenne = (double)somme / (double)compteur;
	seuilFixe(img,res,(int)moyenne);
}
