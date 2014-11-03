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
	std::cout<<"filtre::filtre(3,3)"<<std::endl;

	

}

filtre::filtre(const filtre &origin): filtreH(origin.filtreH), filtreV(origin.filtreV),  
									  img(origin.img), nbLigne(origin.getNbLigne()), nbColonne(origin.getNbColonne())
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

filtre::filtre(const IplImage& image): img(image)
{
	
	nbLigne = nbColonne = 3 ;

	GH.resize(getNbLigne() );
	GV.resize(getNbLigne() );
	Diag.resize(getNbLigne() );
	for (int i = 0; i < getNbLigne(); ++i)
	{
		GH[i].resize(getNbColonne() );
		GV[i].resize(getNbColonne() );
		Diag[i].resize(getNbColonne() );
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

int& filtre::getFiltreH(const unsigned int x, const unsigned int y)
{
	return filtreH[x][y];
}

int filtre::getFiltreH(const unsigned int x, const unsigned int y) const
{
	return filtreH[x][y];
}

int& filtre::getFiltreV(const unsigned int x, const unsigned int y)
{
	return filtreV[x][y];
}

int filtre::getFiltreV(const unsigned int x, const unsigned int y) const
{
	return filtreV[x][y];
}


IplImage filtre::filtreBidirectionnel()
{
	int x,y,i,j;
	int p;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	IplImage res = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 1);

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
	return res;
}

IplImage filtre::filtreBidirectionnelCouleur()
{
	int x,y,i,j;
	int p;
	CvScalar grad;
	double direction;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	IplImage res = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 3);

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
			if(filtreV[y][x] == 0) direction = PI/2;
			else direction = atan2(filtreH[y][x],filtreV[y][x]);
			grad.val[0] = grad.val[1] = grad.val[2] = 0; 
			if(direction >= 0 && direction < PI/2)
			{
				grad.val[0] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);;
			}
			else if(direction >= PI/2 && direction < PI)
			{
				grad.val[1] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);;
			}
			else if(direction < 0 && direction > -PI/2)
			{
				grad.val[2] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);;
			}
			else 
			{
				grad.val[0] = grad.val[1] = grad.val[2] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);;
			}
			cvSet2D(&res,x,y,grad);
		}
	}
	return res;
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

IplImage filtre::filtreMultidirectionnel()
{
	int x,y,i,j;
	int diagG, diagD;
	int p;
	CvScalar grad;
	IplImage res = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 1);
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);

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
	return res;
}

IplImage filtre::filtreMultidirectionnelCouleur()
{
	int x,y,i,j;
	int diagG, diagD;
	int p;
	int maximum;
	CvScalar grad;
	IplImage res = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 3);
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);

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
			
			maximum = max(filtreV[y][x],filtreH[y][x],diagD,diagG);

			grad.val[0] = grad.val[1] = grad.val[2] = 0; 
			if(maximum == filtreV[y][x])
			{
				grad.val[0] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);;
			}
			else if(maximum == filtreH[y][x])
			{
				grad.val[1] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);;
			}
			else if(maximum == diagD)
			{
				grad.val[2] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);;
			}
			else if(maximum == diagG)
			{
				grad.val[0] = grad.val[1] = grad.val[2] = sqrt(filtreH[y][x]*filtreH[y][x] + filtreV[y][x]*filtreV[y][x]);;
			}
			cvSet2D(&res,x,y,grad);
		}
	}
	return res;
}

IplImage filtre::filtreHorizontal()
{
	int x,y,i,j;
	int p;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	IplImage res = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 1);

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
	return res;
}

IplImage filtre::filtreVertical()
{
	int x,y,i,j;
	int p;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	IplImage res = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 1);

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
	return res;
}

IplImage filtre::filtreDiagonalG()
{
	int x,y,i,j;
	int p;
	int S;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	IplImage res = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 1);

	for (x = 1; x < img.height-1; ++x)
	{
		for (y = 1; y < img.width-1; ++y)
		{
			S = 0;
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					p = cvGet2D(&img, x-1+i, y-1+j).val[0]; 
					S += p * Diag[i][j];	
				}
			}
			
			grad.val[0] = S;
			cvSet2D(&res,x,y,grad);
		}
	}
	return res;
}

IplImage filtre::filtreDiagonalD()
{
	int x,y,i,j;
	int p;
	int S;
	CvScalar grad;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	IplImage res = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 1);

	for (x = 1; x < img.height-1; ++x)
	{
		for (y = 1; y < img.width-1; ++y)
		{
			S = 0;
			for (i = 0; i < 3; ++i)
			{
				for (j = 0; j < 3; ++j)
				{
					p = cvGet2D(&img, x-1+i, y-1+j).val[0]; 
					S += p * Diag[2-i][2-j];	
				}
			}
			
			grad.val[0] = S;
			cvSet2D(&res,x,y,grad);
		}
	}
	return res;
}
