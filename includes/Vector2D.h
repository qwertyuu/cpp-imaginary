#pragma once
#include <ostream>

class Vector2D
{
    public:
        Vector2D(double x, double y);
        Vector2D Multiply(double scalar) const;
        Vector2D ToUnit() const;
        double Velocity() const;
        Vector2D ToVelocity(double v) const;
        Vector2D Subtract(Vector2D sub) const;
        Vector2D Add(Vector2D add) const;
        Vector2D Inverse() const;
        double X() const;
        double Y() const;

        friend std::ostream &operator<<(std::ostream &strm, const Vector2D &a)
        {
            return strm << "(" << a.X() << ", " << a.Y() << ")";
        }

    private:
        double x = 0;
        double y = 0;
};
