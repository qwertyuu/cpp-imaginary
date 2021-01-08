#include "Vector2D.h"
#include <gmp.h>

Vector2D::Vector2D(mpf_t &X, mpf_t &Y)
{
    mpf_init(x);
    mpf_set(x, X);

    mpf_init(y);
    mpf_set(y, Y);
}

Vector2D::Vector2D()
{
    mpf_init(x);
    mpf_init(y);
}

Vector2D::~Vector2D()
{
    mpf_clear(x);
    mpf_clear(y);
}

Vector2D *Vector2D::Multiply(double scalar)
{
    mpf_t scalarAsMPF;
    mpf_init_set_d(scalarAsMPF, scalar);
    mpf_mul(x, x, scalarAsMPF);
    mpf_mul(y, y, scalarAsMPF);
    mpf_clear(scalarAsMPF);
    return this;
}

Vector2D *Vector2D::Subtract(Vector2D &sub)
{
    mpf_sub(x, x, sub.x);
    mpf_sub(y, y, sub.y);
    return this;
}

Vector2D *Vector2D::Add(Vector2D &add)
{
    mpf_add(x, x, add.x);
    mpf_add(y, y, add.y);
    return this;
}

void Vector2D::X(mpf_t &X) const
{
    mpf_set(X, x);
}

void Vector2D::Y(mpf_t &Y) const
{
    mpf_set(Y, y);
}