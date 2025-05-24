// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707

#include "graphic.h"

#ifndef TOOLS_H
#define TOOLS_H

constexpr double epsil_zero = 0.5;

struct S2d {
	double x = 0., y = 0.;
	S2d() : x(0), y(0) {}
	S2d(double _x, double _y)
	: x(_x), y(_y) {}
};

struct Polaire {
    double n     = 0;
    double theta = 0; // n: norme, theta: angle en radians

    Polaire(double _n, double _theta)
    : n(_n), theta(_theta) {}
};

struct Cercle {
    S2d centre;
    double rayon = 0;

    Cercle(S2d _centre, double _rayon)
    : centre(_centre), rayon(_rayon) {}
};


//fonctions
Polaire transfo_polaire(S2d p);

S2d transfo_cartesien(Polaire p);

double norme( S2d v, S2d w);

double produitScalaire(S2d a, S2d b);

bool test_inclusion(S2d point,Cercle c1, bool utilisation_epsil_zero);

bool test_intersection(S2d point,Cercle c1, bool utilisation_epsil_zero);

bool test_inclusion(Cercle c1, Cercle c2, bool utilisation_epsil_zero);

bool test_intersection(Cercle c1,Cercle c2,bool utilisation_epsil_zero);

bool test_collision_segment_cercle(S2d A, S2d B, Cercle C);

S2d vect_normal ( S2d contact, Cercle c1);

Polaire vect_reflechi( Polaire& incident, Cercle c1);

S2d projection_cercle(S2d a, double rayon);

S2d oppose_sur_cercle(S2d p);

//fonction affichage grahique
void affichage_cercle_plein_bordure(S2d centre,double radius,Color fill_color,
									Color border_color);
void affichage_cercle_vide_bordure (S2d centre, double radius,Color border_color);

void affichage_segment(S2d A, S2d B, Color color_seg);

#endif // TOOLS_H

