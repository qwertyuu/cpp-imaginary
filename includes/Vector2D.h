#pragma once
#include <ostream>
#include <gmp.h>

class Vector2D
{
    public:
        Vector2D(mpf_t &X, mpf_t &Y);
        Vector2D();
        ~Vector2D();

        Vector2D *Multiply(double scalar);
        Vector2D *Subtract(Vector2D &sub);        
        Vector2D *Add(Vector2D &add);
        void X(mpf_t &X) const;
        void Y(mpf_t &Y) const;

        friend std::ostream &operator<<(std::ostream &strm, const Vector2D &a)
        {
            double _x = mpf_get_d(a.x);
            double _y = mpf_get_d(a.y);
            return strm << "(" << _x << ", " << _y << ")";
        }

    private:
        mpf_t x;
        mpf_t y;
};
