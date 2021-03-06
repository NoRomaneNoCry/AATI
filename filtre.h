/*
CHEMIER Aurélien
LHOMME Romane
*/

#ifndef FILTRE_H
#define FILTRE_H

#include <assert.h>
#include <iostream>
#include <math.h>
#include <opencv/highgui.h>
#include <vector>

class filtre
{
private:
	//filtres
	std::vector<std::vector<int> > GV;
	std::vector<std::vector<int> > GH;
	std::vector<std::vector<int> > Diag;

	//utile pour l'affinage des points
	std::vector<std::vector<int> > filtreH;
	std::vector<std::vector<int> > filtreV;

	//image modele
	IplImage img;

	unsigned int nbLigne;
	unsigned int nbColonne;

public:
	//constructeur / destructeur
	filtre();
	filtre(const filtre &f);
	filtre(const IplImage& image);
	~filtre();

	//getter et setter
	unsigned int& getNbLigne();
	unsigned int& getNbColonne();

	unsigned int getNbLigne() const;
	unsigned int getNbColonne() const;

	int& getGH(const unsigned int x, const unsigned int y);
	int getGH(const unsigned int x, const unsigned int y) const;

	int& getGV(const unsigned int x, const unsigned int y);
	int getGV(const unsigned int x, const unsigned int y) const;

	int& getDiag(const unsigned int x, const unsigned int y);
	int getDiag(const unsigned int x, const unsigned int y) const;

	int& getFiltreH(const unsigned int x, const unsigned int y);
	int getFiltreH(const unsigned int x, const unsigned int y) const;

	int& getFiltreV(const unsigned int x, const unsigned int y);
	int getFiltreV(const unsigned int x, const unsigned int y) const;

	//filtres 
    IplImage filtreHorizontal();
    IplImage filtreVertical();
    
    IplImage filtreDiagonalD();
    IplImage filtreDiagonalG();

    IplImage filtreBidirectionnel();
    IplImage filtreBidirectionnelCouleur();

    IplImage filtreMultidirectionnel();
    IplImage filtreMultidirectionnelCouleur();
};

#endif
