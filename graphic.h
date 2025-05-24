// Théo Marc Petignat Sciper: 398883
// Maximilian Keyan Bayandor Sciper: 391707
#ifndef GRAPHIC_H
#define GRAPHIC_H

enum Color {
    WHITE,
    GREY,
    BLACK,
    RED,
    GREEN,
    BLUE,
    ORANGE,
    PURPLE,
    CYAN
};

void dessin_cercle_vide_bordure(double x, double y, double radius,Color border_color);
void dessin_cercle_plein_bordure(double x, double y,
								 double radius,Color fill_color,Color border_color);
void dessin_segment(double x1, double y1,double x2, double y2,Color color);
#endif
