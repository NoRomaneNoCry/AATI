#ifndef FILTRE_H
#define FILTRE_H
#include <iostream>
#include <vector>
#include <opencv/highgui.h>

class filtre
{
protected:
	std::vector<std::vector<int> > GV;
	std::vector<std::vector<int> > GH;
	std::vector<std::vector<int> > Diag;
	std::vector<std::vector<int> > filtreH;
	std::vector<std::vector<int> > filtreV;

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

	int& getGH(const unsigned int x, const unsigned int y);
	int getGH(const unsigned int x, const unsigned int y) const;

	int& getGV(const unsigned int x, const unsigned int y);
	int getGV(const unsigned int x, const unsigned int y) const;

	int& getDiag(const unsigned int x, const unsigned int y);
	int getDiag(const unsigned int x, const unsigned int y) const;

    void filtreHorizontal(IplImage& img, IplImage& res);
    void filtreVertical(IplImage& img, IplImage& res);
    void filtreBidirectionnel(IplImage& img, IplImage& res);
    void filtreMultidirectionnel(IplImage& img, IplImage& res);


    void seuilFixe(const IplImage& img, IplImage& res, const unsigned int seuil);
    void seuilGlobal(const IplImage& img, IplImage& res);
    void seuilLocal(const IplImage& img, IplImage& res);
    void seuilHysteresis(const IplImage& img, IplImage& res, const int SeuilBas, const int SeuilHaut);
    void seuillageExtractionMaximasLocaux(const IplImage& img, IplImage& res);
};

#endif
