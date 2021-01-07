#include "Vector2D.h"
#include <math.h>

Vector2D::Vector2D(double _x, double _y)
{
    x = _x;
    y = _y;
}

Vector2D Vector2D::Multiply(double scalar) const
{
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::Subtract(Vector2D sub) const
{
    return Vector2D(x - sub.x, y - sub.y);
}

Vector2D Vector2D::Add(Vector2D add) const
{
    return Vector2D(x + add.x, y + add.y);
}

Vector2D Vector2D::ToUnit() const
{
    double velocity = Velocity();
    return Vector2D(x / velocity, y / velocity);
}

Vector2D Vector2D::Inverse() const
{
    return Vector2D(-x, -y);
}

double Vector2D::Velocity() const
{
    return sqrt(x * x + y * y);
}

double Vector2D::X() const
{
    return x;
}

double Vector2D::Y() const
{
    return y;
}

Vector2D Vector2D::ToVelocity(double v) const
{
    return ToUnit().Multiply(v);
}
