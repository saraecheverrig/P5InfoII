#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Vector2D.h"
#include "Particula.h"

class Obstaculo {
private:
    Vector2D posicion; // esquina superior izquierda
    double lado;

public:
    Obstaculo(double x = 0.0, double y = 0.0, double lado = 30.0)
        : posicion(x, y), lado(lado) {}

    double getX() const { return posicion.getX(); }
    double getY() const { return posicion.getY(); }
    double getLado() const { return lado; }

    bool detectarColision(const Particula& p) const;
};

#endif


