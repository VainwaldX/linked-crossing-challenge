// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707
#include <vector>
#include <iostream>
#include "tools.h"

#ifndef CHAINE_H
#define CHAINE_H

class Chaine {
private:
	 std::vector<S2d> articulation;

public:
	Chaine()  = default;
	
	Chaine(std::vector<S2d> temp);
	
	~Chaine() = default;
	
	void set_chaine(const std::vector<S2d>& new_chaine);
	
	std::vector<S2d> get_chaine() const {
        return articulation;
    }
    
	static void detruire_c(std::vector<S2d>& articulation);
};
#endif // CHAINE_H
