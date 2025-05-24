// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707
#ifndef MOBILE_H
#define MOBILE_H

#include <iostream>
#include <string>
#include <vector>
#include "tools.h"


class Mobile {
protected:
   
public:
    S2d position;
    double a = 0;
    double d = 0;
    double rayon;
    Mobile(S2d pos,double _a,double _d, double r)
        : position(pos),a(_a),d(_d), rayon(r) {}
    Mobile(): position(0.,0.),a(0.),d(0.),rayon(0.){}
    
    S2d getPosition() const {
        return position;
    }

    double getRayon() const {
        return rayon;
    }
        
    virtual bool collisionAvec(const Mobile& autre) const;

    virtual ~Mobile() = default;
};

class Particule : public Mobile {
private:
   
public:
    unsigned int compteur; 
    
    Particule(S2d pos, double a,double d,double r, int c)
        : Mobile(pos,a,d, 0), compteur(c) {}
    Particule () : Mobile(), compteur (0) {}
        
    void move_p();
    
    bool verifier_p(std::string& err_mess) const;
};

class Faiseur : public Mobile {
private:
	
public:
	double d_temp;
    unsigned int nbe;
	double longueur;
	bool en_mouvement;
	std::vector<S2d> elements;
	
    Faiseur(S2d pos,double a,double d,double r,double l, unsigned int n,
			std::vector<S2d>el)
	    : Mobile(pos,a,d, r), nbe(n), longueur(l), elements(el) {}
	Faiseur (): Mobile(), nbe(0),longueur(0), en_mouvement(true), elements() {}
		
    const std::vector<S2d>& getElements() const; 
    
    void arreter_faiseur();
    
    void reprendre_mouvement();
    
    void move_f();
    
    bool verifier_f(std::string& err_mess) const;
};
#endif // MOBILE_H
