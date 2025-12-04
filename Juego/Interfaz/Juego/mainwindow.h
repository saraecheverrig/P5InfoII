#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>
#include "Simulacion.h"

class QGraphicsTextItem;
class QGraphicsView;
class QGraphicsEllipseItem;
class QGraphicsRectItem;
class QDoubleSpinBox;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void actualizarSimulacion();
    void lanzarProyectil();
    void reiniciarJuego();

private:
    QGraphicsScene *scene;
    QTimer *timer;
    Simulacion sim;

    // Interfaz
    QGraphicsView *view;
    QDoubleSpinBox *spinAngulo;
    QDoubleSpinBox *spinVelocidad;
    QPushButton *btnLanzar;
    QPushButton *btnReiniciar;
    QLabel *lblEstado;
    QLabel *lblResistencias;
    QLabel *lblTurno;

    // Gráficos
    QGraphicsEllipseItem *proyectilItem;
    std::vector<QGraphicsRectItem*> graficosObstaculos;
    std::vector<QGraphicsTextItem*> textosObstaculos;

    // Estado del juego
    int turnoActual;          // 1 o 2
    bool proyectilEnVuelo;
    bool juegoTerminado;

    void inicializarSimulacion();
    void inicializarEscena();
    void actualizarProyectilGrafico();
    void actualizarResistenciasGrafico();

    void cambiarTurno();
    void actualizarLabelTurno();
    int calcularGanador() const;  // 0 nadie, 1/2 ganador, -1 empate
};
