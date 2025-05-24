// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707

// message.cc  : 13 fonctions pour l'affichage des messages d'erreur
//              et une fonction supplémentaire pour indiquer le succès de la lecture
// 
//
// Version 1.0 du 24.02.2025
//

#include "message.h"

//using namespace std;

// l'espace de nom non-nommé sert ici à restreindre l'usage de la fonction
// reorder_for_consistency à ce module ; c'est une fonction utilitaire locale
namespace { // non-nommé 
    void reorder_for_consistency(unsigned &index1, unsigned &id1,
                                 unsigned &index2, unsigned &id2) {
        if (index1 > index2 || (index1 == index2 && id1 > id2)) {
            std::swap(index1, index2);
            std::swap(id1, id2);
        }
    }
}

// définition des autres fonctions appartenant à l'espace de nom message
// noter la syntaxe " message:: " qui précède le nom de la fonction.
// Il indique que la fonction appartient à cet espace de nom "message" 

std::string message::score_outside(unsigned score) {
    return std::string("score (") + std::to_string(score) 
         + std::string(") must be within ]0, score_max]\n");
}

std::string message::particule_outside(double x, double y) {
    return std::string("particule at (") + std::to_string(x) + std::string(";") 
         + std::to_string(y) + std::string(") is outside the arena\n");
}

std::string message::faiseur_outside(double x, double y) {
    return std::string("faiseur at (") + std::to_string(x) + std::string(";") 
         + std::to_string(y) + std::string(") is outside the arena\n");
}

std::string message::articulation_outside(double x, double y) {
    return std::string("articulation at (") + std::to_string(x) + std::string(";") 
         + std::to_string(y) + std::string(") is outside the arena\n");
}

std::string message::mobile_displacement(double d) {
    return std::string("mobile entity displacement (") + std::to_string(d) 
         + std::string(") must be within [0, d_max]\n");
}

std::string message::nb_particule_outside(unsigned nb) {
    return std::string("particule number (") + std::to_string(nb) 
         + std::string(") must be within [0, nb_particule_max]\n");
}

std::string message::particule_counter(unsigned counter) {
    return std::string("particule counter (") + std::to_string(counter) 
         + std::string(") must be within [0, time_to_spli[ \n");
}

std::string message::faiseur_radius(double radius) {
    return std::string("faiseur radius (") + std::to_string(radius) 
         + std::string(") is not within [r_min_faiseur, r_max_faiseur]\n");
}

std::string message::faiseur_nbe(unsigned nbe) {
    return std::string("faiseur nbe (") + std::to_string(nbe) 
         + std::string(") is not strictly positive\n");
}

std::string message::faiseur_element_collision(unsigned id1, unsigned index1,
								               unsigned id2, unsigned index2) {
    reorder_for_consistency(index1, id1, index2, id2);

    return std::string("faiseur (") + std::to_string(id1)
		 + std::string(") element (") + std::to_string(index1)
		 + std::string(") collides with faiseur (") + std::to_string(id2) 
         + std::string(") element (") + std::to_string(index2) + std::string(")\n");
}

std::string message::chaine_racine(double x, double y) {
    return std::string("root at (") + std::to_string(x) + std::string(";") 
         + std::to_string(y)
         + std::string(") is not close enough to the arena boundary\n");
}

std::string message::chaine_max_distance(unsigned smallest_index) {
    return std::string("too long distance between articulation (") 
         + std::to_string(smallest_index)  
         + std::string(") and the following one\n");
}

std::string message::chaine_articulation_collision(unsigned articulation_index,
	                                          unsigned faiseur_id, 
	                                          unsigned element_index) {
    return std::string("articulation (") + std::to_string(articulation_index) 
         + std::string(") collides with faiseur (") + std::to_string(faiseur_id) 
         + std::string(") element (")
         + std::to_string(element_index) + std::string(")\n");
}

std::string message::success() {
    return std::string("Correct file\n");
}

