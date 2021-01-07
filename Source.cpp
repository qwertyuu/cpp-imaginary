#include <iostream>
#include <string>
#include <curses.h>
#include <fstream>
#include <math.h>
#include "includes/ImaginaryNumber.h"
#include "includes/Vector2D.h"

using namespace std;

bool isMandlebrotBound(ImaginaryNumber C, double ratio)
{
    ImaginaryNumber z = ImaginaryNumber(0, 0);
    if (C.SquaredAbsolute() > 4)
    {
        return false;
    }
    for (int i = 0; i < ratio; i++)
    {
        z = z.Square().Add(C);
        if (z.SquaredAbsolute() > 4)
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
    unsigned int resolution = 512;

    double minImaginary = defaultMinImaginary;
    double maxImaginary = defaultMaxImaginary;

    double zoomFactor = 1;

    while (1)
    {
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
                bool bound = isMandlebrotBound(pixel, resolution);
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
            case 'z': // RESOLUTION +
                resolution *= 2;
                logger << resolution << endl;
                break;
            case 'x': // RESOLUTION -
                resolution /= 2;
                logger << resolution << endl;
                break;
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