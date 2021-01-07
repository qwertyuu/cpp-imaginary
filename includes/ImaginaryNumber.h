#pragma once
#include <ostream>
#include <string>
using namespace std;

class ImaginaryNumber
{
    public:
        ImaginaryNumber(double real_part, double imaginary_part);

        double GetRealPart() const;
        double GetImaginaryPart() const;
        ImaginaryNumber Square() const;
        ImaginaryNumber Add(ImaginaryNumber i) const;
        ImaginaryNumber Subtract(ImaginaryNumber i) const;
        double SquaredAbsolute() const;

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
        double realPart = 0;
        double imaginaryPart = 0;
};

