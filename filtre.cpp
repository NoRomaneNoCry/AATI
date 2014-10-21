#include <assert.h>
#include <math.h>

#include "filtre.h"
#include <iostream>

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

void filtre::appliqueFiltre(IplImage& img, IplImage& res)
{
	int x,y,i,j;
	int Sx, Sy;
	int p;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);

	for (x = 1; x < img.height-1; ++x)
	{
		for (y = 1; y < img.width-1; ++y)
		{
			Sx = 0;
			Sy = 0;
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					p = cvGet2D(&img, x-1+i, y-1+j).val[0]; 
					Sx += p * Gx[i][j];				
					Sy += p * Gy[i][j];
				}
			}
			
			grad.val[0] = sqrt(Sx*Sx + Sy*Sy);
			cvSet2D(&res,x,y,grad);
		}
	}
}

void filtre::seuilFixe(const IplImage& img, IplImage& res, const unsigned int seuil)
{
	int x,y;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);
	CvScalar result;
	for (x = 0; x < img.height; ++x)
	{
		for (y = 0; y < img.width; ++y)
		{
			result = cvGet2D (&img, x, y);
			if(result.val[0] < seuil) result.val[0] = 0;
								 else result.val[0] = 255;

			cvSet2D (&res, x, y,result);
		}
	}
}

void filtre::seuilGlobal(const IplImage& img, IplImage& res)
{
	int x,y;
	double compteur = 0, somme = 0;
	double moyenne;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);

	for (x = 0; x < img.height; ++x)
	{
		for (y = 0; y < img.width; ++y)
		{
			somme += cvGet2D(&img, x, y).val[0];
			compteur ++;
		}
	}

	moyenne = somme / compteur;
	std::cout << moyenne << std::endl;
	seuilFixe(img,res,(int)moyenne);
}

void filtre::seuilLocal(const IplImage& img, IplImage& res)
{
	int x,y,i,j;
	CvScalar result;
	double compteur = 0, somme = 0;
	double moyenne;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);

	for (x = 0; x < img.height; ++x)
	{
		for (y = 0; y < img.width; ++y)
		{
			somme = compteur = 0;
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					somme += cvGet2D(&img, x-1+j, y-1+i).val[0]; 
					compteur ++;
				}
			}
			moyenne = somme / compteur;
			result = cvGet2D (&img, x, y);
			if(result.val[0] < moyenne) result.val[0] = 0;
								 else result.val[0] = 255;

			cvSet2D (&res, x, y,result);
		}
	}
}

void filtre::seuilHysteresis(const IplImage& img, IplImage& res, const int SeuilBas, const int SeuilHaut)
{
	int x,y,i,j;
	int image;
	CvScalar result;

	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);
	assert (SeuilBas <= SeuilHaut);
	
	for (x = 0; x < img.height; ++x)
	{
		for (y = 0; y < img.width; ++y)
		{
			image = cvGet2D(&img, x, y).val[0]; 
			result = cvGet2D(&res, x, y); 
			if(image < SeuilBas)
			{
				result.val[0] = 0;
			}
			else if(image > SeuilHaut)
			{
				result.val[0] = 255;
			}
			else 
			{
				result.val[0] = 2;
			}
			cvSet2D (&res, x, y,result);
		}
	}

	for (x = 1; x < img.height - 1; ++x)
	{
		for (y = 1; y < img.width - 1; ++y)
		{
			result = cvGet2D(&res, x, y); 
			if(result.val[0] == 2)
			{
				for (i = 0; i < 3; ++i)
				{
					for (j = 0; j < 3; ++j)
					{
						if(cvGet2D(&img, x-1+j, y-1+i).val[0] == 255)
						{
							result.val[0] = 255;
							j = i = 3;
						}
						
					}
				}

			}
			cvSet2D (&res, x, y,result);
		}
	}
}
