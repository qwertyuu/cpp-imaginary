#include <iostream>
#include <string>
#include <curses.h>
#include <fstream>
#include <math.h>

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

    private:
        double realPart = 0;
        double imaginaryPart = 0;
};

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

    private:
        double x = 0;
        double y = 0;
};

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

std::ostream &operator<<(std::ostream &strm, const Vector2D &a)
{
    return strm << "(" << a.X() << ", " << a.Y() << ")";
}

bool isMandlebrotBound(ImaginaryNumber C, double ratio)
{
    ImaginaryNumber z = ImaginaryNumber(0, 0);
    for (int i = 0; i < ratio; i++)
    {
        z = z.Square().Add(C);
        ImaginaryNumber distance = z.Subtract(C);
        if (distance.GetRealPart() > ratio * 100 || distance.GetImaginaryPart() > ratio * 100)
        {
            return false;
        }
    }
    return true;
}

int nScreenWidth;
int nScreenHeight;
double defaultMinReal = -2;
double defaultMaxReal = 1;

double defaultMinImaginary = 1;
double defaultMaxImaginary = -1;
ofstream logger;

double interpolateSpan(double min, double span, double progress)
{
    return min + (progress * span);
}

int main()
{  
    logger.open ("log.txt");
    WINDOW *currWin = initscr();
    getmaxyx(currWin, nScreenHeight, nScreenWidth);
    char *screen = new char[nScreenWidth * nScreenHeight];
    double minReal = defaultMinReal;
    double maxReal = defaultMaxReal;

    double minImaginary = defaultMinImaginary;
    double maxImaginary = defaultMaxImaginary;

    double zoomFactor = 1;

    while (1)
    {

        logger << minReal << ", " << maxReal << endl;
        logger << minImaginary << ", " << maxImaginary << endl << endl;
        double realSpan = maxReal - minReal;
        double imaginarySpan = maxImaginary - minImaginary;
    
        for (int y = 0; y < nScreenHeight; y++)
        {
            for (int x = 0; x < nScreenWidth; x++)
            {
                ImaginaryNumber pixel(
                    interpolateSpan(minReal, realSpan, (double)x / (double)nScreenWidth),
                    interpolateSpan(minImaginary, imaginarySpan, (double)y / (double)nScreenHeight)
                );
                bool bound = isMandlebrotBound(pixel, sqrt(1 / realSpan) / 100 + 500);
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
        
        clear();
        mvaddstr(0, 0, screen);
        refresh();

        char input = getch();
        double movement;
        
        switch(input) { // the real value
            case 'w': // UP
                movement = 0.05 * imaginarySpan;
                minImaginary -= movement;
                maxImaginary -= movement;
                break;
            case 's': // DOWN
                movement = 0.05 * imaginarySpan;
                minImaginary += movement;
                maxImaginary += movement;
                break;
            case 'd': // RIGHT
                movement = 0.05 * realSpan;
                minReal += movement;
                maxReal += movement;
                break;
            case 'a': // LEFT
                movement = 0.05 * realSpan;
                minReal -= movement;
                maxReal -= movement;
                break;
            case 'q': // ZOOM+
                {
                    // CENTER POINT
                    double realCenter = minReal + (realSpan / 2.0);
                    double imaginaryCenter = minImaginary + (imaginarySpan / 2.0);
                    Vector2D center(realCenter, imaginaryCenter);

                    Vector2D topLeft(minReal, minImaginary);
                    Vector2D bottomRight(maxReal, maxImaginary);

                    Vector2D newTopLeft = topLeft.Subtract(center).Multiply(0.5).Add(center);
                    Vector2D newBottomRight = bottomRight.Subtract(center).Multiply(0.5).Add(center);

                    minReal = newTopLeft.X();
                    minImaginary = newTopLeft.Y();

                    maxReal = newBottomRight.X();
                    maxImaginary = newBottomRight.Y();
                    logger << center << endl;
                }
                break;
            case 'e': // ZOOM-
                {
                    // CENTER POINT
                    double realCenter = minReal + (realSpan / 2.0);
                    double imaginaryCenter = minImaginary + (imaginarySpan / 2.0);
                    Vector2D center(realCenter, imaginaryCenter);

                    Vector2D topLeft(minReal, minImaginary);
                    Vector2D bottomRight(maxReal, maxImaginary);

                    Vector2D newTopLeft = topLeft.Subtract(center).Multiply(2.0).Add(center);
                    Vector2D newBottomRight = bottomRight.Subtract(center).Multiply(2.0).Add(center);

                    minReal = newTopLeft.X();
                    minImaginary = newTopLeft.Y();

                    maxReal = newBottomRight.X();
                    maxImaginary = newBottomRight.Y();

                    logger << "center: " << center << endl;
                    logger << "topLeft: " << topLeft << endl;
                    logger << "bottomRight: " << bottomRight << endl;
                }
                break;
        }
        logger.flush();
    }
    logger.close();
    endwin();
    return 0;
}

/*
-2, 1 => -0.5, 0

-2  =>  -0.5
1   =>  0

*/