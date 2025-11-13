#include "Particula.h"

void Particula::verificarChoqueConObstaculo(double ox, double oy, double lado, double epsilon) {
    if (posicion.getX() + radio > ox && posicion.getX() - radio < ox + lado &&
        posicion.getY() + radio > oy && posicion.getY() - radio < oy + lado) {

        // Rebote con pérdida de energía (coeficiente de restitución)
        velocidad.setX(-velocidad.getX() * epsilon);
        velocidad.setY(-velocidad.getY() * epsilon);
    }
}

bool Particula::verificarColisionCon(Particula& otra) {
    if (!activa || !otra.activa) return false;

    Vector2D diff = posicion - otra.posicion;
    double distancia = diff.magnitud();

    return distancia < (radio + otra.radio);
}

void Particula::fusionarCon(Particula& otra) {
    if (!activa || !otra.activa) return;

    Vector2D nuevaVel = (velocidad * masa + otra.velocidad * otra.masa) / (masa + otra.masa);
    double nuevaMasa = masa + otra.masa;
    double nuevoRadio = std::sqrt(radio * radio + otra.radio * otra.radio);  // proporcional al área

    velocidad = nuevaVel;
    masa = nuevaMasa;
    radio = nuevoRadio;

    otra.activa = false; // la otra deja de existir
}


