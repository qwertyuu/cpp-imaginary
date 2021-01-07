#include "ImaginaryNumber.h"
#include <string>


ImaginaryNumber::ImaginaryNumber(double real_part, double imaginary_part)
{
    realPart = real_part;
    imaginaryPart = imaginary_part;
}

double ImaginaryNumber::GetRealPart() const
{
    return realPart;
}

double ImaginaryNumber::GetImaginaryPart() const
{
    return imaginaryPart;
}

double ImaginaryNumber::SquaredAbsolute() const
{
    return realPart * realPart + imaginaryPart * imaginaryPart;
}

ImaginaryNumber ImaginaryNumber::Square() const
{
    double squaredRealPart = (realPart * realPart) - (imaginaryPart * imaginaryPart);
    double squaredImaginaryPart = 2 * realPart * imaginaryPart;
    return ImaginaryNumber(squaredRealPart, squaredImaginaryPart);
}

ImaginaryNumber ImaginaryNumber::Add(ImaginaryNumber i) const
{
    return ImaginaryNumber(realPart + i.realPart, imaginaryPart + i.imaginaryPart);
}

ImaginaryNumber ImaginaryNumber::Subtract(ImaginaryNumber i) const
{
    return ImaginaryNumber(realPart - i.realPart, imaginaryPart - i.imaginaryPart);
}