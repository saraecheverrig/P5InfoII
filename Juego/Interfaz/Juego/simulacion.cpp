#include "Simulacion.h"
#include <fstream>
#include <iomanip>

Simulacion::Simulacion(double ancho, double alto, double coefRestObstaculos, double dt, double gravedad, double factorDanio)
    : ancho(ancho), alto(alto), coefRestObstaculos(coefRestObstaculos), dt(dt),
    tiempoActual(0.0), gravedad(gravedad), factorDanio(factorDanio), tiempoMaxDisparo(10.0)
{
}

void Simulacion::agregarParticula(const Particula& p) {
    particulas.push_back(p);
}

void Simulacion::agregarObstaculo(double x, double y, double ancho, double alto, int dueno, double resistencia) {
    obstaculos.emplace_back(x, y, ancho, alto, dueno, resistencia);
}

void Simulacion::paso()
{
    // 1. Mover partículas y manejar colisiones
    for (auto &p : particulas) {
        if (!p.estaActiva()) continue;

        // Aplicar gravedad
        Vector2D vel = p.getVelocidad();
        vel.setY(vel.getY() + gravedad * dt);
        p.setVelocidad(vel);

        Vector2D velAntes = p.getVelocidad();

        p.mover(dt);
        p.verificarChoqueConPared(ancho, alto);

        // Colisión con obstáculos + daño
        for (auto &obs : obstaculos) {
            if (obs.estaDestruido()) continue;

            if (obs.detectarColision(p)) {
                double vMag = velAntes.magnitud();
                double momento = p.getMasa() * vMag;
                double danio = factorDanio * momento;

                obs.aplicarDanio(danio);
                p.verificarChoqueConObstaculo(obs.getX(), obs.getY(), obs.getAncho(), obs.getAlto(), coefRestObstaculos);
            }
        }

        // Fin natural del disparo
        double x = p.getPosicion().getX();
        double y = p.getPosicion().getY();
        double r = p.getRadio();
        double speed = p.getVelocidad().magnitud();

        if (x < -r || x > ancho + r || y > alto + r) {
            p.activa = false;
        }
        if (speed < 5.0) {
            p.activa = false;
        }
    }

    // 2. Colisiones partícula–partícula, en caso de que las necesitemos
    for (std::size_t i = 0; i < particulas.size(); ++i) {
        for (std::size_t j = i + 1; j < particulas.size(); ++j) {
            if (!particulas[i].estaActiva() || !particulas[j].estaActiva())
                continue;

            if (particulas[i].verificarColisionCon(particulas[j])) {
                particulas[i].fusionarCon(particulas[j]);
            }
        }
    }

    // 3. Avanzar tiempo y cortar si se pasó
    tiempoActual += dt;
    if (tiempoActual > tiempoMaxDisparo) {
        for (auto &p : particulas) {
            p.activa = false;
        }
    }
}

void Simulacion::ejecutar(int pasos, const std::string& nombreArchivo)
{
    std::ofstream out(nombreArchivo);
    if (!out.is_open()) return;

    out << std::fixed << std::setprecision(3);

    tiempoActual = 0.0;

    for (int step = 0; step < pasos; ++step) {
        paso();

        out << "t = " << tiempoActual << "\n";
        for (const auto& p : particulas) {
            if (!p.estaActiva()) continue;

            out << "Particula " << p.getId() << ": ("
                << p.getPosicion().getX() << ", "
                << p.getPosicion().getY() << ")\n";
        }
        out << "----------------------------------\n";
    }
}

void Simulacion::reiniciar()
{
    particulas.clear();
    tiempoActual = 0.0;
}

void Simulacion::reiniciarJuego()
{
    for (auto &obs : obstaculos) {
        obs.reiniciar();
    }
    particulas.clear();
    tiempoActual = 0.0;
}

bool Simulacion::hayProyectilActivo() const
{
    for (const auto &p : particulas) {
        if (p.estaActiva()) return true;
    }
    return false;
}
