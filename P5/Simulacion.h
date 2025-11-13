#ifndef SIMULACION_H
#define SIMULACION_H

#include <vector>
#include <fstream>
#include "Particula.h"

class Simulacion {
private:
    double ancho, alto;
    double epsilon;
    double dt;
    std::vector<Particula> particulas;
    std::vector<std::vector<double>> obstaculos;

public:
    Simulacion(double ancho, double alto, double epsilon, double dt);

    void agregarParticula(const Particula& p);
    void agregarObstaculo(double x, double y, double lado);
    void ejecutar(double tiempoTotal, const std::string& nombreArchivo);
};

#endif


