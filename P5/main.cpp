#include <iostream>
#include "Simulacion.h"

int main() {
    Simulacion sim(800, 600, 0.7, 0.1);

    // Agregar partículas
    sim.agregarParticula(Particula(1, 10, 2.0, {100, 100}, {50, 40}));
    sim.agregarParticula(Particula(2, 12, 2.5, {200, 150}, {-40, 30}));
    sim.agregarParticula(Particula(3, 8, 1.5, {400, 300}, {-30, -60}));
    sim.agregarParticula(Particula(4, 9, 2.0, {600, 400}, {20, -50}));

    // Agregar obstáculos (4 cuadrados)
    sim.agregarObstaculo(200, 200, 40);
    sim.agregarObstaculo(400, 100, 50);
    sim.agregarObstaculo(500, 300, 60);
    sim.agregarObstaculo(300, 400, 40);

    sim.ejecutar(10.0, "salida.txt");

    std::cout << "Simulación finalizada. Datos guardados en salida.txt\n";
    return 0;
}



