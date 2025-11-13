#include "Simulacion.h"
#include <iostream>

Simulacion::Simulacion(double ancho, double alto, double epsilon, double dt)
    : ancho(ancho), alto(alto), epsilon(epsilon), dt(dt) {}

void Simulacion::agregarParticula(const Particula& p) {
    particulas.push_back(p);
}

void Simulacion::agregarObstaculo(double x, double y, double lado) {
    obstaculos.push_back({x, y, lado});
}

void Simulacion::ejecutar(double tiempoTotal, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo de salida.\n";
        return;
    }

    double tiempo = 0.0;
    while (tiempo < tiempoTotal) {
        archivo << "t = " << tiempo << "\n";

        for (size_t i = 0; i < particulas.size(); ++i) {
            Particula& p = particulas[i];
            if (!p.estaActiva()) continue;

            p.mover(dt);
            p.verificarChoqueConPared(ancho, alto);

            for (auto& obs : obstaculos)
                p.verificarChoqueConObstaculo(obs[0], obs[1], obs[2], epsilon);

            for (size_t j = i + 1; j < particulas.size(); ++j) {
                Particula& q = particulas[j];
                if (p.verificarColisionCon(q))
                    p.fusionarCon(q);
            }

            archivo << "Partícula " << p.getId()
                    << ": (" << p.getPosicion().getX()
                    << ", " << p.getPosicion().getY() << ")\n";
        }

        archivo << "----------------------------------\n";
        tiempo += dt;
    }

    archivo.close();
}

