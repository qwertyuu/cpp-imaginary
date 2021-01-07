#include "ImaginaryNumber.h"
#include <string>
#include <gmp.h>

ImaginaryNumber::ImaginaryNumber(double real_part, double imaginary_part)
{
    mpf_init(realPart);
    mpf_set_d(realPart, real_part);
    mpf_init(realPartSquared);

    mpf_init(imaginaryPart);
    mpf_set_d(imaginaryPart, imaginary_part);
    mpf_init(imaginaryPartSquared);

    ComputeSquaredParts();
}

ImaginaryNumber::~ImaginaryNumber()
{
    mpf_clear(realPart);
    mpf_clear(imaginaryPart);
    mpf_clear(realPartSquared);
    mpf_clear(imaginaryPartSquared);
}

ImaginaryNumber::ImaginaryNumber(mpf_t &real_part, mpf_t &imaginary_part)
{
    mpf_init(realPart);
    mpf_set(realPart, real_part);
    mpf_init(realPartSquared);

    mpf_init(imaginaryPart);
    mpf_set(imaginaryPart, imaginary_part);
    mpf_init(imaginaryPartSquared);

    ComputeSquaredParts();
}

void ImaginaryNumber::ComputeSquaredParts()
{
    mpf_mul(imaginaryPartSquared, imaginaryPart, imaginaryPart);
    mpf_mul(realPartSquared, realPart, realPart);
}

double ImaginaryNumber::GetRealPart() const
{
    return mpf_get_d(realPart);
}

double ImaginaryNumber::GetImaginaryPart() const
{
    return mpf_get_d(imaginaryPart);
}

bool ImaginaryNumber::SquaredAbsoluteGreaterThan(double v) const
{
    mpf_t realImaginarySquareSum;
    mpf_init(realImaginarySquareSum);
    mpf_add(realImaginarySquareSum, realPartSquared, imaginaryPartSquared);

    bool result = mpf_cmp_d(realImaginarySquareSum, v) > 0;

    mpf_clear(realImaginarySquareSum);

    return result;
}

ImaginaryNumber &ImaginaryNumber::Square()
{
    // tempImaginary = 2 * real * imaginary
    mpf_t tempImaginary;
    mpf_init_set_ui(tempImaginary, 2);
    mpf_mul(tempImaginary, tempImaginary, realPart);
    mpf_mul(tempImaginary, tempImaginary, imaginaryPart);

    // real = real^2 - imaginary^2
    mpf_sub(realPart, realPartSquared, imaginaryPartSquared);

    // imaginary = tempImaginary
    mpf_set(imaginaryPart, tempImaginary);
    mpf_clear(tempImaginary);

    ComputeSquaredParts();
    return *this;
}

ImaginaryNumber &ImaginaryNumber::Add(ImaginaryNumber &i)
{
    mpf_add(realPart, realPart, i.realPart);
    mpf_add(imaginaryPart, imaginaryPart, i.imaginaryPart);

    ComputeSquaredParts();
    return *this;
}

ImaginaryNumber &ImaginaryNumber::Subtract(ImaginaryNumber &i)
{
    mpf_sub(realPart, realPart, i.realPart);
    mpf_sub(imaginaryPart, imaginaryPart, i.imaginaryPart);

    ComputeSquaredParts();
    return *this;
}