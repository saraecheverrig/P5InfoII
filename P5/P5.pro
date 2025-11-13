TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Obstaculo.cpp \
        Particula.cpp \
        Simulacion.cpp \
        main.cpp

HEADERS += \
    Obstaculo.h \
    Particula.h \
    Simulacion.h \
    Vector2D.h
