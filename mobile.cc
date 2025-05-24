// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707

#include <iostream>
#include <cmath>
#include "mobile.h"
#include "constantes.h"
#include "tools.h"
#include "message.h"


// Définition des méthodes de Mobile
bool Mobile::collisionAvec(const Mobile& autre) const {
    double distance = norme(position, autre.position);
    return distance < (rayon + autre.rayon);
}

// Méthode getElements pour Faiseur
const std::vector<S2d>& Faiseur::getElements() const {
    return elements;
}

void Particule::move_p() {
	if (d > 0) {
        position.x += d * cos(a);
        position.y += d * sin(a);
    }
}

bool Particule::verifier_p(std::string& err_mess) const {
	if (compteur >= time_to_split) {
		err_mess = message::particule_counter(compteur);
		return false;
	}
	
    if (!test_inclusion(position, Cercle(S2d(0, 0), r_max), 0)) {
		err_mess = message::particule_outside(position.x, position.y);
        return false;
    }
    if (d < 0 || d >= d_max) {
        err_mess = message::mobile_displacement(d);
        return false;
    }
    return true;
}

void Faiseur::arreter_faiseur() {
    if (en_mouvement == false) {
        d_temp = d;
        d 	   = 0;
    }
}

void Faiseur::reprendre_mouvement() {
    if (en_mouvement == true && d == 0) {
		d = d_temp; 
    }
}

void Faiseur::move_f() {
	if (en_mouvement) {
        position.x += d * cos(a);
        position.y += d * sin(a);
    }
}

bool Faiseur::verifier_f(std::string& err_mess) const {
	if (d < 0 || d >= d_max) {
        err_mess = message::mobile_displacement(d);
        return false;
    }
    if (rayon < r_min_faiseur || rayon > r_max_faiseur) {
        err_mess = message::faiseur_radius(rayon);
        return false;
    }
    if (nbe == 0) {
        err_mess = message::faiseur_nbe(nbe);
        return false;
    }
    if (!test_inclusion(Cercle(position, rayon), Cercle(S2d(0, 0), r_max), 0)) {
        err_mess = message::faiseur_outside(position.x, position.y);
        return false;
    }
    return true;
}
