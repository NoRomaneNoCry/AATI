#include "seuillage.h"

seuillage::seuillage(){}

seuillage::seuillage(IplImage image): img(image)
{
	resSeuillage = *cvCreateImage(cvGetSize(&img), IPL_DEPTH_8U, 1);
}

seuillage::~seuillage(){}

IplImage seuillage::seuilFixe(unsigned int seuil)
{
	int x,y;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	if(seuil > 255) seuil = 255;
	CvScalar result;

	for (x = 0; x < img.height; ++x)
	{
		for (y = 0; y < img.width; ++y)
		{
			result = cvGet2D (&img, x, y);
			if(result.val[0] < seuil) result.val[0] = 0;
								 else result.val[0] = 255;

			cvSet2D (&resSeuillage, x, y,result);
		}
	}
	return resSeuillage;
}

IplImage seuillage::seuilGlobal()
{
	int x,y;
	double compteur = 0, somme = 0;
	double moyenne;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);

	for (x = 0; x < img.height; ++x)
	{
		for (y = 0; y < img.width; ++y)
		{
			somme += cvGet2D(&img, x, y).val[0];
			compteur ++;
		}
	}

	moyenne = somme / compteur;
	return seuilFixe((int)moyenne);
}

IplImage seuillage::seuilLocal()
{
	int x,y,i,j;
	CvScalar result;
	double compteur = 0, somme = 0;
	double moyenne;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);

	for (x = 1; x < img.height-1; ++x)
	{
		for (y = 1; y < img.width-1; ++y)
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

			cvSet2D (&resSeuillage, x, y,result);
		}
	}
	return resSeuillage;
}


IplImage seuillage::seuilHysteresis(int unsigned SeuilBas, int unsigned SeuilHaut)
{
	int x,y,i,j;
	int image;
	CvScalar result;

	if(SeuilHaut > 255) SeuilHaut = 255;
	if(SeuilBas> 255) SeuilBas = 255;
	assert (img.depth == IPL_DEPTH_8U && img.nChannels == 1);
	if (SeuilBas > SeuilHaut)
	{
		unsigned int t = SeuilBas;
		SeuilBas = SeuilHaut;
		SeuilHaut = t;
	}
	
	for (x = 0; x < img.height; ++x)
	{
		for (y = 0; y < img.width; ++y)
		{
			image = cvGet2D(&img, x, y).val[0]; 
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
			cvSet2D (&resSeuillage, x, y,result);
		}
	}

	for (x = 1; x < img.height - 1; ++x)	
	{
		for (y = 1; y < img.width - 1; ++y)
		{
			result = cvGet2D(&resSeuillage, x, y); 
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
			cvSet2D (&resSeuillage, x, y,result);
		}
	}
	return resSeuillage;
}

IplImage seuillage::affinage(const filtre& f)
{
	double GM1, GM2;
	int G;
	double gH,gV;
	CvScalar s;
	IplImage res = *cvCreateImage(cvGetSize(&resSeuillage), IPL_DEPTH_8U, 1);

	for (int i = 1; i < img.height-1; i++)
	{
		for (int j = 1; j < img.width-1; j++)
		{
			G = cvGet2D(&resSeuillage,i,j).val[0]; 
		
			gH = f.getFiltreH(j,i);
			gV = f.getFiltreV(j,i);
			if(gV > gH)
			{
				GM1 = cvGet2D(&resSeuillage,i-1,j).val[0];
				GM2 =  cvGet2D(&resSeuillage,i+1,j).val[0];
				if(GM1 <= G && G > GM2)
				{
					s.val[0] = 255;
					cvSet2D(&res,i,j, s);
				}
			}
			else if(gV < gH)
			{
				GM1 =  cvGet2D(&resSeuillage,i,j+1).val[0];
				GM2 =  cvGet2D(&resSeuillage,i,j-1).val[0];
				if(GM1 <= G && G > GM2)
				{
					s.val[0] = 255;
					cvSet2D(&res,i,j, s);
				}
			}

			

					
		}
	}
	return res;
}

