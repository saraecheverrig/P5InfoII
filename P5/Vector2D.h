#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

class Vector2D {
private:
    double x, y;

public:
    Vector2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    double getX() const { return x; }
    double getY() const { return y; }

    void setX(double nuevoX) { x = nuevoX; }
    void setY(double nuevoY) { y = nuevoY; }

    Vector2D operator+(const Vector2D& otro) const {
        return Vector2D(x + otro.x, y + otro.y);
    }

    Vector2D operator-(const Vector2D& otro) const {
        return Vector2D(x - otro.x, y - otro.y);
    }

    Vector2D operator*(double escalar) const {
        return Vector2D(x * escalar, y * escalar);
    }

    Vector2D operator/(double escalar) const {
        return Vector2D(x / escalar, y / escalar);
    }

    double magnitud() const {
        return std::sqrt(x * x + y * y);
    }

    void normalizar() {
        double m = magnitud();
        if (m > 0) {
            x /= m;
            y /= m;
        }
    }

    double productoPunto(const Vector2D& otro) const {
        return x * otro.x + y * otro.y;
    }
};

#endif



