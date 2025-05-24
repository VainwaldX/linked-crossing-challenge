// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707
#ifndef JEU_H
#define JEU_H

#include <string>
#include "constantes.h"
#include "mobile.h" 
#include "tools.h"
#include "chaine.h"

enum Status {
    ONGOING,
    WON,
    LOST
};

struct game_state {
	
	std::string ligne;
	std::string s_mode;
	std::string err_mess;

	unsigned int nombre_particules, nombre_faiseurs, score, nb_articulations;

	int particules_rest, faiseurs_rest, articulations_rest;

	bool score_lu, nbpart_lu, nbfais_lu, nb_arti_lu, collisions, erreur_presente,
		 coll_ff_detectee, coll_af_detectee, succes;

	std::vector<double> game_data;
	std::vector<Particule> particules;
	std::vector<Faiseur> faiseurs;
	std::vector<S2d> tempo;
	
	S2d point_objectif;
	
	Chaine chainejeu;
	
	Mode mode;
	
	Status status;
};

game_state* get_state();

void init_reset();

void verif_nb_particule(const unsigned int& nb_part);

void verif_particules(const Particule& p);

void verif_faiseurs(const Faiseur& f);

void verif_score(const unsigned int& score);

void verif_articulation(const std::vector<S2d>& temp);

//vérifie les collisons entre les articulations et les faiseurs
void verif_collisions_af(const std::vector<S2d>& articulations,
						 bool mode_individuel, const std::vector<Faiseur>& faiseurs,
						 size_t index_faiseur);
						 
//vérifie les collisons entre les faiseurs
void verif_collisions_ff(std::vector<Faiseur>& faiseurs, size_t index_faiseur,
						 bool mode_individuel);
						 
void lecture(const std::string& nom_fichier);

void sauvegarde(const std::string& nomFichier);

//change le type
void string_to_mode(const std::string& s_mode, Mode& mode);

void execution();

//split une particule en deux nouvelles
void p_split(Particule& new_part1, Particule& new_part2, size_t& index);

//s'occupe du cas lorsqu'il y a une collsion entre les articulations et les 
//faiseurs
void coll_af(std::vector<S2d>& tempo);

//algo guidage, rendu3
void guidage(); 

//vérifie succès
void success(bool& succes); 

void fin_jeu_s();


// fonctions d'affichage
void affichage_chaine(Chaine A);

void affichage_particules(std::vector<Particule> B);

void affichage_faiseurs(std::vector<Faiseur> C);

void affichage_jeu(); //affiche tous les éléments 

#endif // JEU_H
