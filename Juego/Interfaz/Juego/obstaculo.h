#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Vector2D.h"
#include "Particula.h"

class Obstaculo {
private:
    Vector2D posicion;   // esquina superior izquierda
    double ancho;
    double alto;
    double resistencia;      // vida actual
    double resistenciaMax;   // vida máxima
    int dueno;               // 1 o 2 (jugador)

public:
    Obstaculo(double x = 0.0, double y = 0.0, double ancho = 30.0, double alto = 30.0,
              int dueno = 0, double resistencia = 100.0)
        : posicion(x, y), ancho(ancho), alto(alto), resistencia(resistencia),
        resistenciaMax(resistencia), dueno(dueno)
    {}

    double getX() const { return posicion.getX(); }
    double getY() const { return posicion.getY(); }
    double getAncho() const { return ancho; }
    double getAlto() const { return alto; }

    double getResistencia() const { return resistencia; }
    double getResistenciaMax() const { return resistenciaMax; }
    int getDueno() const { return dueno; }

    bool estaDestruido() const { return resistencia <= 0.0; }

    void aplicarDanio(double d) {
        resistencia -= d;
        if (resistencia < 0.0) resistencia = 0.0;
    }

    void reiniciar() {
        resistencia = resistenciaMax;
    }

    bool detectarColision(const Particula& p) const;
};

#endif
