#include "Obstaculo.h"

bool Obstaculo::detectarColision(const Particula& p) const {
    double px = p.getPosicion().getX();
    double py = p.getPosicion().getY();

    if (px + p.getRadio() > posicion.getX() &&
        px - p.getRadio() < posicion.getX() + lado &&
        py + p.getRadio() > posicion.getY() &&
        py - p.getRadio() < posicion.getY() + lado) {
        return true;
    }
    return false;
}



