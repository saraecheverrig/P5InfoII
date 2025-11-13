#ifndef PARTICULA_H
#define PARTICULA_H

#include "Vector2D.h"

class Particula {
private:
    int id;
    double radio;
    double masa;
    Vector2D posicion;
    Vector2D velocidad;
    bool activa;

public:
    Particula(int id = 0, double radio = 5.0, double masa = 1.0, Vector2D pos = {}, Vector2D vel = {})
        : id(id), radio(radio), masa(masa), posicion(pos), velocidad(vel), activa(true) {}

    int getId() const { return id; }
    double getRadio() const { return radio; }
    double getMasa() const { return masa; }
    bool estaActiva() const { return activa; }

    Vector2D getPosicion() const { return posicion; }
    Vector2D getVelocidad() const { return velocidad; }

    void setPosicion(const Vector2D& p) { posicion = p; }
    void setVelocidad(const Vector2D& v) { velocidad = v; }

    // Movimiento básico
    void mover(double dt) {
        posicion = posicion + velocidad * dt;
    }

    // Choque con pared (perfectamente elástico)
    void verificarChoqueConPared(double ancho, double alto) {
        if (posicion.getX() - radio < 0 || posicion.getX() + radio > ancho)
            velocidad.setX(-velocidad.getX());
        if (posicion.getY() - radio < 0 || posicion.getY() + radio > alto)
            velocidad.setY(-velocidad.getY());
    }

    // Choque con obstáculo cuadrado (inelástico)
    void verificarChoqueConObstaculo(double ox, double oy, double lado, double epsilon);

    // Choque entre partículas (completamente inelástico)
    bool verificarColisionCon(Particula& otra);

    void fusionarCon(Particula& otra);
};

#endif


