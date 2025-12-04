#include "mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPainter>

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>

#include <QBrush>
#include <QPen>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    scene(new QGraphicsScene(this)),
    timer(new QTimer(this)),
    sim(800, 600, 0.7, 0.016),
    view(nullptr),
    spinAngulo(nullptr),
    spinVelocidad(nullptr),
    btnLanzar(nullptr),
    btnReiniciar(nullptr),
    lblEstado(nullptr),
    lblResistencias(nullptr),
    lblTurno(nullptr),
    proyectilItem(nullptr),
    turnoActual(1),
    proyectilEnVuelo(false),
    juegoTerminado(false)
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *layoutPrincipal = new QHBoxLayout(central);

    // Vista de la escena
    view = new QGraphicsView(scene, central);
    view->setRenderHint(QPainter::Antialiasing);
    view->setSceneRect(0, 0, 800, 600);
    layoutPrincipal->addWidget(view, 1);

    // Panel lateral
    auto *panel = new QWidget(central);
    auto *panelLayout = new QVBoxLayout(panel);

    lblTurno = new QLabel(tr("Turno: Jugador 1"), panel);

    auto *lblAng = new QLabel(tr("Ángulo (grados):"), panel);
    spinAngulo = new QDoubleSpinBox(panel);
    spinAngulo->setRange(0.0, 90.0);
    spinAngulo->setValue(45.0);
    spinAngulo->setSingleStep(1.0);

    auto *lblVel = new QLabel(tr("Velocidad inicial:"), panel);
    spinVelocidad = new QDoubleSpinBox(panel);
    spinVelocidad->setRange(0.0, 300.0);
    spinVelocidad->setValue(80.0);
    spinVelocidad->setSingleStep(5.0);

    btnLanzar = new QPushButton(tr("Lanzar"), panel);
    btnReiniciar = new QPushButton(tr("Reiniciar juego"), panel);
    lblEstado = new QLabel(tr("Listo"), panel);
    lblResistencias = new QLabel(tr("Resistencias:"), panel);

    panelLayout->addWidget(lblTurno);
    panelLayout->addSpacing(10);
    panelLayout->addWidget(lblAng);
    panelLayout->addWidget(spinAngulo);
    panelLayout->addWidget(lblVel);
    panelLayout->addWidget(spinVelocidad);
    panelLayout->addWidget(btnLanzar);
    panelLayout->addWidget(btnReiniciar);
    panelLayout->addSpacing(10);
    panelLayout->addWidget(lblEstado);
    panelLayout->addWidget(lblResistencias);
    panelLayout->addStretch();

    layoutPrincipal->addWidget(panel);

    inicializarSimulacion();
    inicializarEscena();
    actualizarResistenciasGrafico();

    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarSimulacion);
    timer->start(16);

    connect(btnLanzar, &QPushButton::clicked, this, &MainWindow::lanzarProyectil);
    connect(btnReiniciar, &QPushButton::clicked, this, &MainWindow::reiniciarJuego);
}

MainWindow::~MainWindow() = default;

// Escenario
void MainWindow::inicializarSimulacion()
{
    double colWidth  = 70.0;
    double colHeight = 150.0;
    double roofHeight = 60.0;
    double groundY = 580.0;                // parte baja de las columnas
    double colTopY = groundY - colHeight;  // parte alta de las columnas
    double roofTopY = colTopY - roofHeight;

    // Base izquierda (Jugador 1)
    double base1X = 80.0;

    // columnas (200)
    sim.agregarObstaculo(base1X, colTopY, colWidth, colHeight, 1, 200.0); // izquierda
    sim.agregarObstaculo(base1X + colWidth, colTopY, colWidth, colHeight, 1, 200.0); // centro (Rival)
    sim.agregarObstaculo(base1X + 2*colWidth, colTopY, colWidth, colHeight, 1, 200.0); // derecha
    // techo (100) sobre las tres columnas
    sim.agregarObstaculo(base1X, roofTopY, 3*colWidth, roofHeight, 1, 100.0);

    // Base derecha (Jugador 2)
    double base2X = 800.0 - 80.0 - 3*colWidth;

    sim.agregarObstaculo(base2X, colTopY, colWidth, colHeight, 2, 200.0); // izquierda
    sim.agregarObstaculo(base2X + colWidth, colTopY, colWidth, colHeight, 2, 200.0); // centro (Rival)
    sim.agregarObstaculo(base2X + 2*colWidth, colTopY, colWidth, colHeight, 2, 200.0); // derecha
    sim.agregarObstaculo(base2X, roofTopY, 3*colWidth, roofHeight, 2, 100.0);
}

void MainWindow::inicializarEscena()
{
    // Borde de la caja
    scene->addRect(0, 0, 800, 600);

    // Cañón izquierdo (Jugador 1)
    {
        // base (no necesitamos guardar el puntero)
        scene->addRect(10, 290, 25, 20, QPen(Qt::black), QBrush(Qt::darkGray));
        // cañón (tubo)
        auto *barrel1 = scene->addRect(20, 280, 30, 8, QPen(Qt::black), QBrush(Qt::blue));
        barrel1->setTransformOriginPoint(20, 280 + 4); // centro del tubo
        barrel1->setRotation(-20); // apuntando hacia el centro
    }

    // Cañón derecho (Jugador 2)
    {
        // base
        scene->addRect(800 - 35, 290, 25, 20, QPen(Qt::black), QBrush(Qt::darkGray));
        // cañón (tubo)
        auto *barrel2 = scene->addRect(800 - 50, 280, 30, 8, QPen(Qt::black), QBrush(Qt::blue));
        barrel2->setTransformOriginPoint(800 - 20, 280 + 4);
        barrel2->setRotation(20);  // apunta hacia el centro
    }

    // Dibujar los obstáculos
    const auto &obstaculos = sim.getObstaculos();
    graficosObstaculos.clear();
    textosObstaculos.clear();

    for (std::size_t i = 0; i < obstaculos.size(); ++i) {
        const auto &obs = obstaculos[i];
        double x = obs.getX();
        double y = obs.getY();
        double w = obs.getAncho();
        double h = obs.getAlto();

        // Colores de las bases
        QColor color;
        if (obs.getResistenciaMax() == 100.0) {
            color = QColor(230, 230, 230);          // techo gris claro
        } else { // 200
            if (i == 1 || i == 5)  // bloques centrales (rival)
                color = Qt::white;
            else
                color = QColor(255, 230, 180);      // columnas naranja claro
        }

        auto *rectItem = scene->addRect(x, y, w, h, QPen(Qt::black), QBrush(color));
        graficosObstaculos.push_back(rectItem);

        // Texto con la vida máxima (100 o 200)
        QGraphicsTextItem *txt = scene->addText(QString::number((int)obs.getResistenciaMax()));
        QRectF br = txt->boundingRect();
        txt->setPos(x + (w - br.width()) / 2.0, y + 5);
        textosObstaculos.push_back(txt);

        // Dibujar rival sobre los bloques centrales del jugador 2
        if (i == 1 || i == 5) {
            double cx = x + w / 2.0;
            double top = y + 40.0;
            double headR = 10.0;

            // Cabeza
            scene->addEllipse(cx - headR, top, 2 * headR, 2 * headR, QPen(Qt::black));

            // Cuerpo
            double bodyTop = top + 2 * headR;
            double bodyBottom = bodyTop + 30.0;
            scene->addLine(cx, bodyTop, cx, bodyBottom, QPen(Qt::black));

            // Brazos
            scene->addLine(cx, bodyTop + 10.0, cx - 15.0, bodyTop + 25.0, QPen(Qt::black));
            scene->addLine(cx, bodyTop + 10.0, cx + 15.0, bodyTop + 25.0, QPen(Qt::black));

            // Piernas
            scene->addLine(cx, bodyBottom, cx - 15.0, bodyBottom + 20.0, QPen(Qt::black));
            scene->addLine(cx, bodyBottom, cx + 15.0, bodyBottom + 20.0, QPen(Qt::black));

            // Texto "Rival"
            QGraphicsTextItem *txtR = scene->addText("Rival");
            QRectF br2 = txtR->boundingRect();
            txtR->setPos(cx - br2.width() / 2.0, bodyBottom + 22.0);
        }
    }
}


void MainWindow::actualizarSimulacion()
{
    if (juegoTerminado) return;

    sim.paso();
    actualizarProyectilGrafico();
    actualizarResistenciasGrafico();

    int ganador = calcularGanador();
    if (ganador != 0) {
        juegoTerminado = true;
        btnLanzar->setEnabled(false);

        if (ganador == 1)      lblEstado->setText("¡Ganó el Jugador 1!");
        else if (ganador == 2) lblEstado->setText("¡Ganó el Jugador 2!");
        else                   lblEstado->setText("¡Empate, ambas infraestructuras destruidas!");
        return;
    }

    if (proyectilEnVuelo && !sim.hayProyectilActivo()) {
        proyectilEnVuelo = false;
        cambiarTurno();
    }
}

void MainWindow::actualizarProyectilGrafico()
{
    const auto &particulas = sim.getParticulas();

    if (particulas.empty()) {
        if (proyectilItem) proyectilItem->setVisible(false);
        return;
    }

    const Particula &p = particulas[0];
    if (!p.estaActiva()) {
        if (proyectilItem) proyectilItem->setVisible(false);
        return;
    }

    double r = p.getRadio();
    Vector2D pos = p.getPosicion();
    double x = pos.getX();
    double y = pos.getY();

    if (!proyectilItem) {
        proyectilItem = scene->addEllipse(x - r, y - r, 2 * r, 2 * r, QPen(Qt::black), QBrush(Qt::red));
    } else {
        proyectilItem->setRect(x - r, y - r, 2 * r, 2 * r);
        proyectilItem->setVisible(true);
    }
}

void MainWindow::actualizarResistenciasGrafico()
{
    const auto &obstaculos = sim.getObstaculos();

    QString texto = "Resistencias:\n";
    for (std::size_t i = 0; i < obstaculos.size(); ++i) {
        const auto &obs = obstaculos[i];
        double rActual = obs.getResistencia();
        double rMax = obs.getResistenciaMax();

        texto += QString("Obs %1 (J%2): %3 / %4\n").arg(i + 1).arg(obs.getDueno()).arg(rActual, 0, 'f', 1).arg(rMax, 0, 'f', 1);

        bool visible = !obs.estaDestruido();

        if (i < graficosObstaculos.size()) {
            graficosObstaculos[i]->setVisible(visible);
        }
        if (i < textosObstaculos.size()) {
            textosObstaculos[i]->setVisible(visible);
        }
    }

    lblResistencias->setText(texto);
}

void MainWindow::lanzarProyectil()
{
    if (juegoTerminado) return;
    if (proyectilEnVuelo) return;

    sim.reiniciar();
    if (proyectilItem) proyectilItem->setVisible(false);

    double anguloGrados = spinAngulo->value();
    double velocidad    = spinVelocidad->value();

    constexpr double PI = 3.14159265358979323846;
    double anguloRad = anguloGrados * PI / 180.0;

    double vx;
    double vy = -velocidad * std::sin(anguloRad); // hacia arriba

    // Disparar desde los cañones en la mitad de los laterales
    double startX;
    double startY = 300.0;

    if (turnoActual == 1) {
        startX = 40.0;                 // cerca del cañón izquierdo
        vx = velocidad * std::cos(anguloRad);
    } else {
        startX = 800.0 - 40.0;         // cerca del cañón derecho
        vx = -velocidad * std::cos(anguloRad);
    }

    Particula proyectil(1, 8.0, 5.0, Vector2D(startX, startY), Vector2D(vx, vy));

    sim.agregarParticula(proyectil);

    actualizarProyectilGrafico();
    actualizarResistenciasGrafico();

    proyectilEnVuelo = true;

    lblEstado->setText(tr("Jugador %1 disparó: ángulo %2°, v0 = %3").arg(turnoActual).arg(anguloGrados).arg(velocidad));
}

void MainWindow::cambiarTurno()
{
    turnoActual = (turnoActual == 1) ? 2 : 1;
    actualizarLabelTurno();

    lblEstado->setText(tr("Turno terminado. Ahora juega el Jugador %1").arg(turnoActual));
}

void MainWindow::actualizarLabelTurno()
{
    lblTurno->setText(tr("Turno: Jugador %1").arg(turnoActual));
}

int MainWindow::calcularGanador() const
{
    const auto &obstaculos = sim.getObstaculos();

    bool todos1Destruidos = true;
    bool todos2Destruidos = true;

    for (const auto &obs : obstaculos) {
        if (obs.getDueno() == 1 && !obs.estaDestruido()) todos1Destruidos = false;
        if (obs.getDueno() == 2 && !obs.estaDestruido()) todos2Destruidos = false;
    }

    if (todos1Destruidos && !todos2Destruidos) return 2;
    if (todos2Destruidos && !todos1Destruidos) return 1;
    if (todos1Destruidos && todos2Destruidos)  return -1;
    return 0;
}

void MainWindow::reiniciarJuego()
{
    sim.reiniciarJuego();

    juegoTerminado = false;
    proyectilEnVuelo = false;
    turnoActual = 1;
    actualizarLabelTurno();

    btnLanzar->setEnabled(true);

    if (proyectilItem) {
        proyectilItem->setVisible(false);
    }

    actualizarResistenciasGrafico();
    lblEstado->setText("Juego reiniciado. Turno del Jugador 1.");
}
