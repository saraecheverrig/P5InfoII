#ifndef SIMULACION_H
#define SIMULACION_H

#include <vector>
#include <string>
#include "Particula.h"
#include "Obstaculo.h"

class Simulacion
{
private:
    double ancho;
    double alto;
    double coefRestObstaculos;  // epsilon de choques con obstáculos
    double dt;                  // paso de tiempo
    double tiempoActual;        // tiempo del disparo actual

    double gravedad;            // aceleración hacia abajo
    double factorDanio;         // daño = k * m * |v|
    double tiempoMaxDisparo;    // duración máxima de un disparo

    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;

public:
    Simulacion(double ancho, double alto, double coefRestObstaculos, double dt,
               double gravedad = 9.8, double factorDanio = 0.05);

    void agregarParticula(const Particula& p);
    void agregarObstaculo(double x, double y, double ancho, double alto,
                          int dueno, double resistencia = 100.0);

    void ejecutar(int pasos, const std::string& nombreArchivo);

    void paso();

    void reiniciar();      // solo proyectil
    void reiniciarJuego(); // proyectil + estructuras

    const std::vector<Particula>& getParticulas() const { return particulas; }
    const std::vector<Obstaculo>& getObstaculos() const { return obstaculos; }
    double getTiempoActual() const { return tiempoActual; }

    bool hayProyectilActivo() const;
};

#endif
