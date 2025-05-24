// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cmath>
#include "tools.h"
#include "graphic.h"

bool utilisation_epsil_zero (false);

// Fonction pour normaliser un angle dans [-π, π]
double normaliserAngle(double angle) {
    while (angle > M_PI) angle   -= 2 * M_PI;
    while (angle < - M_PI) angle += 2 * M_PI;
    return angle;
}
// Conversion entre cartésien et polaire
Polaire transfo_polaire(S2d p) {
	Polaire z(0,0);
	z.n 	= sqrt(p.x * p.x + p.y * p.y);
	z.theta = normaliserAngle(atan2(p.y, p.x));
    return z;
}

// Conversion entre  polaire et cartésien
S2d transfo_cartesien(Polaire p) {
    return S2d(p.n *cos(p.theta), p.n *sin(p.theta));
}

// Calcul de la norme d'un vecteur
double norme( S2d v, S2d w) {
	double dx = (v.x-w.x);
	double dy = (v.y-w.y);
	return sqrt(dx*dx + dy*dy);
}

// test d'intersection et d'inclusion avec un cercle

// point - cercle
bool test_inclusion(S2d point,Cercle c1,bool utilisation_epsil_zero) {
	if (utilisation_epsil_zero) return norme(point,c1.centre) <
									  (c1.rayon - epsil_zero);
	else return(norme(point,c1.centre) < c1.rayon);
}

bool test_intersection(S2d point,Cercle c1, bool utilisation_epsil_zero) {
	if (utilisation_epsil_zero) return norme(point,c1.centre) == c1.rayon -
									   epsil_zero;
     else return norme(point,c1.centre) 					  == c1.rayon;
}
	
//cercle-cercle
bool test_inclusion(Cercle c1, Cercle c2, bool utilisation_epsil_zero) {
	if (utilisation_epsil_zero) return norme(c1.centre,c2.centre) < (c2.rayon -
									   c1.rayon - epsil_zero);
	else return norme(c1.centre,c2.centre) < (c2.rayon - c1.rayon);
}

bool test_intersection(Cercle c1,Cercle c2, bool utilisation_epsil_zero) {
	if (utilisation_epsil_zero) return norme(c1.centre,c2.centre) < (c1.rayon +
									   c2.rayon + epsil_zero);
	else return norme(c1.centre,c2.centre) < (c1.rayon + c2.rayon);	 
}


S2d vect_normal ( S2d contact, Cercle c1) {
	S2d no(0,0);
	no.x = contact.x - c1.centre.x;
	no.y = contact.y - c1.centre.y;
	return no;
}
 
// modification du vecteur incident après rebond
Polaire vect_reflechi( Polaire& incident, Cercle c1) {
	S2d cart 	   = transfo_cartesien(incident);
	cart.x 		  -= c1.centre.x;
	cart.y  	  -= c1.centre.y;
	Polaire normal = transfo_polaire(vect_normal(cart,c1));
	incident.theta = normaliserAngle(M_PI + 2* normal.theta - incident.theta);
	return incident;
}


//produit scalaire de deux vecteurs
double produitScalaire(S2d a, S2d b) {
    return a.x * b.x + a.y * b.y;
}

// test collision/inclusion d'un segment et d'un cercle
bool test_collision_segment_cercle(S2d A, S2d B, Cercle C) {
	
    //Si l'un des points du segment est à l'intérieur du cercle
    if (test_inclusion(A,C,false) or test_inclusion(B,C,false)) return true;
    
    // Vérif intersection avec le cercle par projection sur la droite d'AB
    S2d AB = {B.x - A.x,B.y - A.y};
    S2d AC = {C.centre.x - A.x, C.centre.y - A.y};

    double normeAB2		   = produitScalaire(AB, AB);
    double projectionAC_AB = produitScalaire(AC, AB);

    // paramètre t de la projection orthogonale
    double t = projectionAC_AB / normeAB2;

    S2d pointProjete = {A.x + t * AB.x, A.y + t * AB.y};

    //distance point projeté-centre du cercle
    double distanceProjection = norme(pointProjete, C.centre);

    if (distanceProjection <= C.rayon) return true;

    return false;
}
// projecton sur le cercle de centre (0,0)
S2d projection_cercle(S2d a, double rayon) {
    double n;
    double lambda;
    S2d b;
	n = norme(a, S2d(0, 0));
    if (n == 0) {
        // Cas particulier : vecteur nul,retourne un point par défaut
        b.x = rayon;
        b.y = 0;
        
    } else {
        lambda = rayon / n;
        b.x = lambda * a.x;
        b.y = lambda * a.y;
    }

    return b;
}
// opposé sur le cercle
S2d oppose_sur_cercle(S2d p) {
    S2d opp;
    opp.x = -p.x;
    opp.y = -p.y;
    return opp;
}


	

void affichage_cercle_plein_bordure(S2d centre, double radius, Color fill_color,
									Color border_color) {
	dessin_cercle_plein_bordure(centre.x,centre.y,radius,fill_color,border_color);
}
void affichage_cercle_vide_bordure (S2d centre, double radius, Color border_color) {
	dessin_cercle_vide_bordure(centre.x, centre.y, radius, border_color);
}
void affichage_segment(S2d A, S2d B, Color color_seg) {
	dessin_segment(A.x,A.y,B.x,B.y,color_seg);
}
