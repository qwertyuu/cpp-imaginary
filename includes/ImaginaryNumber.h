#pragma once
#include <ostream>
#include <string>
#include <gmp.h>
using namespace std;

class ImaginaryNumber
{
    public:
        ImaginaryNumber(double real_part, double imaginary_part);
        ImaginaryNumber(mpf_t &real_part, mpf_t &imaginary_part);

        ~ImaginaryNumber();

        double GetRealPart() const;
        double GetImaginaryPart() const;
        ImaginaryNumber &Square();
        ImaginaryNumber &Add(ImaginaryNumber &i);
        ImaginaryNumber &Subtract(ImaginaryNumber &i);
        bool SquaredAbsoluteGreaterThan(double v) const;

        friend std::ostream &operator<<(std::ostream &strm, const ImaginaryNumber &a)
        {
            double iP = a.GetImaginaryPart();
            string sIP = "+ " + to_string(iP) + "i";
            if (iP < 0)
            {
                sIP = "- " + to_string(-iP) + "i";
            }
            return strm << "(" << a.GetRealPart() << " " << sIP << ")";
        }

    private:
        mpf_t realPart;
        mpf_t imaginaryPart;
        mpf_t realPartSquared;
        mpf_t imaginaryPartSquared;

        void ComputeSquaredParts();
};

