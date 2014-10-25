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
	GH.resize(nbLigne);
	GV.resize(nbLigne);
	Diag.resize(nbLigne);
	for (int i = 0; i < nbLigne; ++i)
	{
		GH[i].resize(nbColonne);
		GV[i].resize(nbColonne);
		Diag[i].resize(nbColonne);
	}
}

filtre::filtre(const filtre &origin): filtreH(origin.filtreH), filtreV(origin.filtreV), nbLigne(origin.getNbLigne()), nbColonne(origin.getNbColonne())
{
	GH.resize(nbLigne);
	GV.resize(nbLigne);
	Diag.resize(nbLigne);
	for (int i = 0; i < nbLigne; ++i)
	{
		GH[i].resize(nbColonne);
		GV[i].resize(nbColonne);
		Diag[i].resize(nbColonne);
		for (int j = 0; j < nbColonne; ++j)
		{
			GH[i][j] = origin.getGH(i,j);
			GV[i][j] = origin.getGV(i,j);
			Diag[i][j] = origin.getDiag(i,j);
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

int& filtre::getGV(const unsigned int x, const unsigned int y)
{
	return GV[x][y];
}

int filtre::getGV(const unsigned int x, const unsigned int y) const
{
	return GV[x][y];
}

int& filtre::getGH(const unsigned int x, const unsigned int y)
{
	return GH[x][y];
}

int filtre::getGH(const unsigned int x, const unsigned int y) const
{
	return GH[x][y];
}

int& filtre::getDiag(const unsigned int x, const unsigned int y)
{
	return Diag[x][y];
}
	
int filtre::getDiag(const unsigned int x, const unsigned int y) const
{
	return Diag[x][y];
}

void filtre::filtreBidirectionnel(IplImage& img, IplImage& res)
{
	int x,y,i,j;
	int p;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);

	filtreH.resize(img.width);
	filtreV.resize(img.width);
	for (int i = 0; i < img.width; ++i)
	{
		filtreH[i].resize(img.height);
		filtreV[i].resize(img.height);
	}

	for (x = 1; x < img.height-1; ++x)
	{
		for (y = 1; y < img.width-1; ++y)
		{
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					p = cvGet2D(&img, x-1+i, y-1+j).val[0]; 
					filtreH[y][x] += p * GH[i][j];				
					filtreV[y][x] += p * GV[i][j];
				}
			}
			
			grad.val[0] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);
			cvSet2D(&res,x,y,grad);
		}
	}
}

static int max(const int a, const int b, const int c, const int d)
{
	if(a > b && a > c && a > d)
		return a;
	if(b > c && b > d)
		return b;
	if(c > d)
		return c;
	return d;
}

void filtre::filtreMultidirectionnel(IplImage& img, IplImage& res)
{
	int x,y,i,j;
	int diagG, diagD;
	int p;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);

	filtreH.resize(img.width);
	filtreV.resize(img.width);
	for (int i = 0; i < img.width; ++i)
	{
		filtreH[i].resize(img.height);
		filtreV[i].resize(img.height);
	}

	for (x = 1; x < img.height-1; ++x)
	{
		for (y = 1; y < img.width-1; ++y)
		{
			diagD = diagG = 0;
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					p = cvGet2D(&img, x-1+i, y-1+j).val[0]; 
					filtreH[y][x] += p * GH[i][j];				
					filtreV[y][x] += p * GV[i][j];
					diagG += p* Diag[i][j];
					diagD += p* Diag[2-i][2-j];
				}
			}
			
			grad.val[0] = max(filtreV[y][x],filtreH[y][x],diagD,diagG);
			cvSet2D(&res,x,y,grad);
		}
	}
}

void filtre::filtreHorizontal(IplImage& img, IplImage& res)
{
	int x,y,i,j;
	int p;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);

	filtreH.resize(img.width);
	for (int i = 0; i < img.width; ++i)
	{
		filtreH[i].resize(img.height);
	}

	for (x = 1; x < img.height-1; ++x)
	{
		for (y = 1; y < img.width-1; ++y)
		{
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					p = cvGet2D(&img, x-1+i, y-1+j).val[0]; 
					filtreH[y][x] += p * GH[i][j];	
				}
			}
			
			grad.val[0] = filtreH[y][x];
			cvSet2D(&res,x,y,grad);
		}
	}
}

void filtre::filtreVertical(IplImage& img, IplImage& res)
{
	int x,y,i,j;
	int p;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	assert (res.depth == IPL_DEPTH_8U && res.nChannels == 1);

	filtreV.resize(img.width);
	for (int i = 0; i < img.width; ++i)
	{
		filtreV[i].resize(img.height);
	}
	for (x = 1; x < img.height-1; ++x)
	{
		for (y = 1; y < img.width-1; ++y)
		{
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					p = cvGet2D(&img, x-1+i, y-1+j).val[0]; 
					filtreV[y][x] += p * GV[i][j];	
				}
			}
			
			grad.val[0] = filtreV[y][x];
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

void filtre::seuillageExtractionMaximasLocaux(const IplImage& img, IplImage& res)
{
	double GM1, GM2;
	int G;
	double gH,gV;
	CvScalar s;
	res = *cvCloneImage(&img);

	for (int i = 1; i < img.height-1; i++)
	{
		for (int j = 1; j < img.width-1; j++)
		{
			G = cvGet2D(&img,i,j).val[0]; 
		
			gH = filtreH[i][j];
			gV = filtreV[i][j];
			if(abs(gV) >= abs(gH))
			{
				if(gV == 0) gV = 1;
				GM1 = (gH / gV) * cvGet2D(&img,i-1,j).val[0] + ((gV - gH) / gV) * cvGet2D(&img,i-1,j+1).val[0];
				GM2 = (gH / gV) * cvGet2D(&img,i+1,j-1).val[0] + ((gV - gH) / gV) * cvGet2D(&img,i+1,j).val[0];
			}
			else if(abs(gV) < abs(gH))
			{
				if(gH == 0) gH = 1;
				GM1 = (gV / gH) * cvGet2D(&img,i,j+1).val[0] + ((gH - gV) / gH) * cvGet2D(&img,i-1,j+1).val[0];
				GM2 = (gV / gH) * cvGet2D(&img,i,j-1).val[0] + ((gH - gV) / gH) * cvGet2D(&img,i+1,j-1).val[0];
			}
			else GM1 = 1000000;
			if(!(G > GM1 && G > GM2))
			{
				s.val[0] = 0;
				cvSet2D(&res,i,j, s);
			}				
		}
	}
}
