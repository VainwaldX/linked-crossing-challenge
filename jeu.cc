// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707

#include <sstream>
#include <iostream>
#include <limits>
#include <fstream>
#include <vector>
#include <string>
#include "constantes.h"
#include "message.h"
#include "tools.h"
#include "chaine.h"
#include "mobile.h"
#include "jeu.h"

static game_state etat;

Chaine chainejeu;

Mode mode;

game_state* get_state() {
	return &etat;
}

void init_reset() {
	etat.ligne 				= "";
	etat.s_mode 			= "";
	etat.err_mess 			= "";
	etat.nombre_particules  = 0;
	etat.nombre_faiseurs 	= 0;
	etat.score 				= 8000;
	etat.nb_articulations 	= 0;
	etat.particules_rest 	= 0;
	etat.faiseurs_rest 		= 0;
	etat.articulations_rest = 0;
	etat.score_lu 			= false;
	etat.nbpart_lu 			= false;
	etat.nb_arti_lu 		= false;
	etat.collisions 		= false;
	etat.erreur_presente 	= false;
	etat.coll_ff_detectee 	= false;
	etat.coll_af_detectee 	= false;
	etat.succes 			= false;
	etat.particules.clear();
	etat.faiseurs.clear();
	etat.tempo.clear();
	etat.game_data.clear();
}

void verif_nb_particule(const unsigned int& nb_part) {
    if (nb_part > nb_particule_max) {
        std::cout << message::nb_particule_outside(nb_part);
        etat.erreur_presente = true;
		etat.game_data.clear();
        return;
    }
    etat.game_data.push_back(nb_part);
}

void verif_particules(const Particule& p) {
	if (!p.verifier_p(etat.err_mess)) {
		std::cout << etat.err_mess;
        etat.erreur_presente = true;
        etat.game_data.clear();
        return;
	}
	
	etat.game_data.push_back(p.position.x);
    etat.game_data.push_back(p.position.y);
    etat.game_data.push_back(p.a);
    etat.game_data.push_back(p.d);
    etat.game_data.push_back(p.compteur);
}

void verif_faiseurs(const Faiseur& f) {
    if (!f.verifier_f(etat.err_mess)) {
        std::cout << etat.err_mess;
        etat.erreur_presente = true;
        etat.game_data.clear();
        return;
    }
    etat.game_data.push_back(f.position.x);
    etat.game_data.push_back(f.position.y);
    etat.game_data.push_back(f.a);
    etat.game_data.push_back(f.d);
    etat.game_data.push_back(f.rayon);
    etat.game_data.push_back(f.nbe);
}

void verif_score(const unsigned int& score) {
	etat.game_data.push_back(score);
    if (score > score_max || score == 0) {
        std::cout << message::score_outside(score);
        etat.game_data.clear();
        etat.erreur_presente = true;
        return;
    }
}

void verif_articulation(const std::vector<S2d>& temp) {
    S2d dernier_point(0, 0);
    int dernier_point_index = 0;
    for (size_t i = 0; i < temp.size(); i++) {
        if (!test_inclusion(temp[i], Cercle(S2d(0, 0), r_max), false)) {
            std::cout << message::articulation_outside(temp[i].x, temp[i].y);
            etat.game_data.clear();
            etat.erreur_presente = true;
			return;
        }
        if (i == 0) {
            dernier_point = temp[0];
            if (r_max - norme(temp[0], S2d(0, 0)) > r_capture) {
                std::cout << message::chaine_racine(temp[0].x, temp[0].y);
                etat.game_data.clear();
                etat.erreur_presente = true;
				return;
            }
        } else {
            if (norme(dernier_point, temp[i]) > r_capture) {
                std::cout << message::chaine_max_distance(dernier_point_index);
                etat.game_data.clear();
                etat.erreur_presente = true;
				return;
            }
            dernier_point = temp[i];
            dernier_point_index += 1;
        }
    }
}

void verif_collisions_af(const std::vector<S2d>& articulations,
						 bool mode_individuel, const std::vector<Faiseur>& faiseurs,
						 size_t index_faiseur) {
	etat.coll_af_detectee = false;
	if (mode_individuel) {
		for (size_t j = 0; j < articulations.size(); ++j) {
            for (size_t k = 0; k < faiseurs[index_faiseur].elements.size(); ++k) {
                if (norme(articulations[j], faiseurs[index_faiseur].elements[k]) <=
					faiseurs[index_faiseur].rayon) {
                    etat.coll_af_detectee = true;
                    return;
                }
            }
        }
	} else {
		for (size_t i = 0; i < faiseurs.size(); ++i) {
			for (size_t j = 0; j < articulations.size(); ++j) {
				for (size_t k = 0; k < faiseurs[i].elements.size(); ++k) {
					if (norme(articulations[j], faiseurs[i].elements[k]) <=
						faiseurs[i].rayon) {
						etat.coll_af_detectee = true;
						std::cout << message::chaine_articulation_collision(j,i,k);
						etat.game_data.clear();
						etat.erreur_presente = true;
						return;
					}
				}
			}
		}
	}
}

void verif_collisions_ff(std::vector<Faiseur>& faiseurs, size_t index_faiseur,
						 bool mode_individuel) {
	etat.coll_ff_detectee = false;
    if (mode_individuel) {
        for (size_t j = 0; j < faiseurs.size(); ++j) {
            if (index_faiseur == j) continue;
            for (size_t k = 0; k < faiseurs[index_faiseur].elements.size(); ++k) {
                for (size_t l = 0; l < faiseurs[j].elements.size(); ++l) {
                    double faiseur_collision_distance =
						   faiseurs[index_faiseur].rayon + faiseurs[j].rayon;
                    if (norme(faiseurs[index_faiseur].elements[k],
						faiseurs[j].elements[l]) <= faiseur_collision_distance) {
                        etat.coll_ff_detectee = true;
                        return;
                    }
                }
            }
        }
    } else {
        for (size_t i = 0; i < faiseurs.size(); ++i) {
            for (size_t j = i + 1; j < faiseurs.size(); ++j) {
                for (size_t k = 0; k < faiseurs[i].elements.size(); ++k) {
                    for (size_t l = 0; l < faiseurs[j].elements.size(); ++l) {
                        double faiseur_collision_distance = faiseurs[i].rayon +
							   faiseurs[j].rayon;
                        if (norme(faiseurs[i].elements[k], faiseurs[j].elements[l])
							<= faiseur_collision_distance) {
                            etat.coll_ff_detectee = true;
                            std::cout<<message::faiseur_element_collision(i,k,j,l);
                            etat.game_data.clear();
							etat.erreur_presente = true;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void lecture(const std::string& nom_fichier) {
	init_reset();
    std::ifstream fichier(nom_fichier);
    while (getline(fichier, etat.ligne)) {
        if (etat.ligne.empty() || etat.ligne[0] == '\n' ||
			etat.ligne[0] == '\r' || etat.ligne[0] == '#') continue;
        std::stringstream iss(etat.ligne);
        if (etat.ligne == "CONSTRUCTION" || etat.ligne == "GUIDAGE") {
			etat.s_mode = etat.ligne;
			break;
		}
        if (!etat.score_lu) {
            iss >> etat.score;
            verif_score(etat.score);
            if (etat.erreur_presente) break;
            etat.score_lu = true;
            continue;
        }
        if (!etat.nbpart_lu) {
            iss >> etat.nombre_particules;
            verif_nb_particule(etat.nombre_particules);
            if (etat.erreur_presente) break;
            etat.particules_rest = etat.nombre_particules;
            etat.nbpart_lu = true;
            continue;
        }
        if (etat.particules_rest > 0) {
            Particule p;
            iss >> p.position.x >> p.position.y >> p.a >> p.d >> p.compteur;
            verif_particules(p);
            if (etat.erreur_presente) break;
            etat.particules.push_back(p);
            etat.particules_rest -= 1;
            continue;
        }
        if (!etat.nbfais_lu) {
            iss >> etat.nombre_faiseurs;
            etat.faiseurs_rest = etat.nombre_faiseurs;
            etat.nbfais_lu = true;
            etat.game_data.push_back(etat.nombre_faiseurs);
            continue;
        }
        if (etat.faiseurs_rest > 0) {
            Faiseur f;
            iss >> f.position.x >> f.position.y >> f.a >> f.d >> f.rayon >> f.nbe;
            verif_faiseurs(f);
            if (etat.erreur_presente) break;
            etat.faiseurs.push_back(f);
            etat.faiseurs_rest -= 1;
            continue;
        }
        if (!etat.nb_arti_lu) {
            iss >> etat.nb_articulations;
            etat.articulations_rest = etat.nb_articulations;
            etat.nb_arti_lu = true;
            etat.game_data.push_back(etat.nb_articulations);
            continue;
        }
        if (etat.articulations_rest > 0) {
            S2d position;
            iss >> position.x >> position.y;
            etat.tempo.push_back(position);
            etat.articulations_rest -= 1;
            etat.game_data.push_back(position.x);
            etat.game_data.push_back(position.y);
            continue;
        }
        if (!etat.collisions) {
			verif_collisions_af(etat.tempo, false, etat.faiseurs, 0);
			verif_collisions_ff(etat.faiseurs, 0, false);
			if (etat.erreur_presente) break;
			etat.collisions = true;
			continue;
		}
    }
    chainejeu.set_chaine(etat.tempo);
    verif_articulation(etat.tempo);
    if (etat.erreur_presente) init_reset();
    std::cout << message::success();
    sauvegarde(nom_fichier);
}	
//Attention, max. lignes est 80; 79 lignes sans compter début/fin

void sauvegarde(const std::string& nomFichier) {
	std::ofstream fichier(nomFichier);
	fichier << "# Nom du scenario de test \n # \n" << etat.score << "\n"
			"\n # nombre d’entité particule puis les données d’une entité"
			" par ligne \n" << etat.nombre_particules << "\n";
	for (size_t i = 0; i < etat.nombre_particules; i++) {
		fichier << etat.particules[i].position.x << " "
		<< etat.particules[i].position.y << " " << etat.particules[i].a << " "
		<< etat.particules[i].d << " " << etat.particules[i].compteur << "\n";
	}
	fichier << "\n # nombre d’entité faiseur puis les données d’une entité par " 
			"ligne \n" << etat.nombre_faiseurs << "\n";
	for (size_t i = 0; i < etat.nombre_faiseurs; i++) {
		fichier << etat.faiseurs[i].position.x << " "
		<< etat.faiseurs[i].position.y << " " << etat.faiseurs[i].a << " "
		<< etat.faiseurs[i].d << " " << etat.faiseurs[i].rayon << " "
		<< etat.faiseurs[i].nbe << "\n";
	}
	fichier << "# nombre d’articulations (nul veut dire « pas de chaîne ») \n"
			"		 puis une ligne par articulation \n" << etat.nb_articulations
			<< "\n";
	for (size_t i = 0; i < etat.nb_articulations; i++) {
		fichier << etat.tempo[i].x << " " << etat.tempo[i].y << "\n";
	}
	fichier << etat.s_mode << "\n";
}

void string_to_mode(const std::string& s_mode, Mode& mode) {
    if (s_mode == "CONSTRUCTION") mode = CONSTRUCTION;
    else if (s_mode == "GUIDAGE") mode = GUIDAGE;
}

void p_split(Particule& new_part1, Particule& new_part2, size_t& index) {
	new_part1 = etat.particules[index];
	new_part2 = etat.particules[index];
	new_part1.a += delta_split;
	new_part2.a -= delta_split;
	new_part1.d *= coef_split;
	new_part2.d *= coef_split;
	etat.particules.erase(etat.particules.begin() + index);
	index--;
	etat.particules.push_back(new_part1);
	etat.particules.push_back(new_part2);
}


void coll_af(std::vector<S2d>& tempo) { 
	chainejeu.~Chaine();
}

// UNE mise à jour du jeu (tant que le score est strictement positif, sinon fin du jeu
// avec échec)

// entrée modifiée : ensemble des entités
void execution() {
	if (etat.score < 0) init_reset(); //score =0 => won/Lost    //exit game but idk how yet, need to change this
	etat.score--;
	for (size_t i = 0; i < etat.particules.size(); i++) {
		etat.particules[i].compteur++;
		if (etat.particules[i].compteur == time_to_split) {
			if (etat.nombre_particules == nb_particule_max) {
				etat.particules.erase(etat.particules.begin() + i); 
				i--;
			} else {
				Particule new_part1, new_part2;
				p_split(new_part1, new_part2, i);
			}
		}
		etat.particules[i].move_p();
		// ajouter le rebond sur l'arène 
	}
	for (size_t i = 0; i < etat.faiseurs.size(); i++) {
		verif_collisions_ff(etat.faiseurs, i, true);
		if (etat.coll_ff_detectee) etat.faiseurs[i].arreter_faiseur();
		else if(!etat.faiseurs[i].en_mouvement) etat.faiseurs[i].reprendre_mouvement(); 
		etat.faiseurs[i].move_f();
		// ajouter le rebond sur l'arène 
		verif_collisions_af(etat.tempo, true, etat.faiseurs, i);
		if (etat.coll_af_detectee) coll_af(etat.tempo);
	}
	if (mode == GUIDAGE) {
		//guidage(); //algo guidag, rendu3
		verif_collisions_af(etat.tempo, false, etat.faiseurs, 0);
		if (etat.coll_af_detectee) coll_af(etat.tempo);
		//success(succes); //rendu3
		//if (succes) //fin_jeu_s();
	}
}

//affichage des éléments
void affichage_chaine(Chaine A) {
	std::vector<S2d> temp = A.get_chaine();
	for (size_t i = 0; i < temp.size(); ++i) {
		if(i < temp.size()-1) affichage_segment(temp[i], temp[i+1], RED);
		affichage_cercle_vide_bordure(temp[i], r_viz,RED);
		if(i == temp.size()-1) affichage_cercle_vide_bordure(temp[i+1], r_capture, RED);
	}
}

void affichage_particules(std::vector<Particule> B) {
	for (size_t i = 0; i < B.size(); ++i) {
		affichage_cercle_plein_bordure(B[i].position, r_viz, CYAN, GREEN);
	}
}

void affichage_faiseurs(std::vector<Faiseur> C) {
	for (size_t i = 0; i < C.size(); ++i) {
        for (size_t j = 0; j < C[i].elements.size(); ++i) {
			affichage_cercle_vide_bordure(C[i].elements[j], C[i].rayon, BLUE);
		}
	}
}	
void affichage_objectif(S2d point){
	affichage_cercle_vide_bordure(point, r_viz,BLACK);
}

void affichage_jeu(){
	affichage_cercle_vide_bordure(S2d(0,0), r_max,GREEN);
	affichage_particules(etat.particules);
	affichage_faiseurs(etat.faiseurs);
	affichage_chaine(etat.chainejeu);
	/*S2d a;                                            
	a = projection_cercle(etat.chainejeu[0], r_max);
	point_objectif = oppose_sur_cercle(a);
	affichage_objectif(etat.point_objectif);
	*/                                              //Sûrement à supprimer
}
//

// cas construction
void construction(S2d pos_souris,bool left_click_pressed ) {
	if (etat.chainejeu.set_chaine.size() ==0) {
		S2d a;
		S2d b;
		a = projection_cercle(pos_souris, r_max);
		b = oppose_sur_cercle(a);
		affichage_cercle_vide_bordure(a,r_capture,RED);
		affichage_objectif(b);
	}
	else {
		if (left_click_pressed) {
			
		}
		
	}
} 


void right_click(Mode mode, bool &right_click_pressed) {
    if (mode == GUIDAGE) {
        right_click_pressed = true;
    } else { 
        mode = GUIDAGE;
        right_click_pressed = true;
    }
}








