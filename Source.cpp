#include <iostream>
#include <string>
#include <curses.h>

using namespace std;

class ImaginaryNumber
{
public:
    ImaginaryNumber(double real_part, double imaginary_part);
    ~ImaginaryNumber();

    double GetRealPart() const;
    double GetImaginaryPart() const;
    ImaginaryNumber Square() const;
    ImaginaryNumber Add(ImaginaryNumber i) const;
    ImaginaryNumber Subtract(ImaginaryNumber i) const;

private:
    double realPart = 0;
    double imaginaryPart = 0;
};

ImaginaryNumber::ImaginaryNumber(double real_part, double imaginary_part)
{
    realPart = real_part;
    imaginaryPart = imaginary_part;
}

ImaginaryNumber::~ImaginaryNumber()
{
    // no op
}

double ImaginaryNumber::GetRealPart() const
{
    return realPart;
}

double ImaginaryNumber::GetImaginaryPart() const
{
    return imaginaryPart;
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

std::ostream &operator<<(std::ostream &strm, const ImaginaryNumber &a)
{
    double iP = a.GetImaginaryPart();
    string sIP = "+ " + to_string(iP) + "i";
    if (iP < 0)
    {
        sIP = "- " + to_string(-iP) + "i";
    }
    return strm << "(" << a.GetRealPart() << " " << sIP << ")";
}

bool isMandlebrotBound(ImaginaryNumber C)
{
    ImaginaryNumber z = ImaginaryNumber(0, 0);
    for (int i = 0; i < 5000; i++)
    {
        z = z.Square().Add(C);
        ImaginaryNumber distance = z.Subtract(C);
        if (distance.GetRealPart() > 1000000 || distance.GetImaginaryPart() > 1000000)
        {
            return false;
        }
    }
    return true;
}

int nScreenWidth = 120;
int nScreenHeight = 40;

int main()
{
    char *screen = new char[nScreenWidth*nScreenHeight];
    for (int y = 0; y < nScreenHeight; y++)
    {
        for (int x = 0; x < nScreenWidth; x++)
        {
            ImaginaryNumber une(-2 + (((double)x) / ((double)nScreenWidth) * 3.0), 1 + (((double)y) / ((double)nScreenHeight) * -2.0));
            bool bound = isMandlebrotBound(une);
            if (bound)
            {
                screen[y * nScreenWidth + x] = 'A';
            }
            else
            {
                screen[y * nScreenWidth + x] = ' ';
            }
            
        }
    }
    for (int y = 0; y < nScreenHeight; y++)
    {
        for (int x = 0; x < nScreenWidth; x++)
        {
            if (screen[y * nScreenWidth + x] > 0)
                cout << screen[y * nScreenWidth + x];
            else
                cout << ' ';
        }
    }
    

    initscr();
    clear();
    mvaddstr(0, 0, screen);
    refresh();
    getch();
    endwin();
    return 0;
}