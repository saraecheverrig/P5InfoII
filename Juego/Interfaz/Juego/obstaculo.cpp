#include "Obstaculo.h"

bool Obstaculo::detectarColision(const Particula& p) const {
    double px = p.getPosicion().getX();
    double py = p.getPosicion().getY();

    // Verificar solapamiento círculo–rectángulo (AABB)
    if (px + p.getRadio() > posicion.getX() &&
        px - p.getRadio() < posicion.getX() + ancho &&
        py + p.getRadio() > posicion.getY() &&
        py - p.getRadio() < posicion.getY() + alto) {
        return true;
    }
    return false;
}
