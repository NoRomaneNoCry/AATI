#ifndef FILTRE_H
#define FILTRE_H
#include <iostream>
#include <vector>
#include <opencv/highgui.h>

class filtre
{
protected:
	std::vector<std::vector<int> > Gx;
	std::vector<std::vector<int> > Gy;
	unsigned int nbLigne;
	unsigned int nbColonne;

public:
	filtre();
	filtre(const unsigned int ligne,const unsigned int colonne);
	filtre(const filtre &f);
	~filtre();

	unsigned int& getNbLigne();
	unsigned int& getNbColonne();

	unsigned int getNbLigne() const;
	unsigned int getNbColonne() const;

	int& getGy(const unsigned int x, const unsigned int y);
	int getGy(const unsigned int x, const unsigned int y) const;

	int& getGx(const unsigned int x, const unsigned int y);
	int getGx(const unsigned int x, const unsigned int y) const;

    filtre& operator=(filtre f);

    void appliqueFiltre(IplImage& img, IplImage& res);
    void appliqueFiltreCouleur(IplImage& img, IplImage& res);

    void seuilFixe(const IplImage& img, IplImage& res, const unsigned int seuil);
    void seuilGlobal(const IplImage& img, IplImage& res);
};

#endif
