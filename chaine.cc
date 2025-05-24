// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707

#include <cmath>
#include <vector>
#include <iostream>
#include "tools.h"
#include "constantes.h"
#include "message.h"
#include "chaine.h"

Chaine::Chaine(std::vector<S2d> temp) : articulation(temp) {}

void Chaine::set_chaine(const std::vector<S2d>& new_chaine) {
	articulation = new_chaine;
}

//détruis la chaine
void Chaine::detruire_c(std::vector<S2d>& articulation) {
	articulation.clear();
}



