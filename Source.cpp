#include <iostream>
#include <string>
#include <curses.h>
#include <fstream>
#include <math.h>
#include "includes/ImaginaryNumber.h"
#include "includes/Vector2D.h"
#include <gmp.h>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <tuple>
#include <atomic>

using namespace std;

bool isMandlebrotBound(ImaginaryNumber &C, double ratio)
{
    if (C.SquaredAbsoluteGreaterThan(4))
    {
        return false;
    }
    ImaginaryNumber z(0.0, 0.0);
    for (int i = 0; i < ratio; i++)
    {
        z = z.Square().Add(C);
        if (z.SquaredAbsoluteGreaterThan(4))
        {
            return false;
        }
    }
    return true;
}

int nScreenWidth;
int nScreenHeight;
mpf_t defaultMinReal;
mpf_t defaultMaxReal;
mpf_t defaultMinImaginary;
mpf_t defaultMaxImaginary;
int resolutionStep = 128;
std::ostream &operator<<(std::ostream &strm, const mpf_t &a)
{
    return strm << "(mpf_t: " << mpf_get_d(a) << ")";
}

ofstream logger;

void interpolateSpan(mpf_t &min, mpf_t &span, mpf_t &progress, mpf_t &result)
{
    mpf_mul(result, span, progress);
    mpf_add(result, result, min);
}

std::mutex mtx;
std::condition_variable cv;
std::queue<tuple<int, int, int, int>> Queue;
char *screen;
mpf_t screenWidth;
mpf_t screenHeight;
mpf_t minReal;
mpf_t maxReal;
mpf_t minImaginary;
mpf_t maxImaginary;
mpf_t realSpan;
mpf_t imaginarySpan;
mpf_t movement;
unsigned int resolution = 128;
int zoomFactor = 0;

atomic<int> sectionsComputed;
atomic<int> totalSectionsToCompute;

void computeMandlebrotSection()
{
    while (true)
    {
        tuple<int, int, int, int> Job;
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [] { return !Queue.empty(); });
            Job = Queue.front();
            Queue.pop();
        }
        double mandlebrotIterations = zoomFactor * 45 + 500.0;

        for (int y = get<2>(Job); y < get<3>(Job); y++)
        {
            for (int x = get<0>(Job); x < get<1>(Job); x++)
            {
                mpf_t widthProgress;
                mpf_init_set_si(widthProgress, x);
                mpf_div(widthProgress, widthProgress, screenWidth);

                mpf_t heightProgress;
                mpf_init_set_si(heightProgress, y);
                mpf_div(heightProgress, heightProgress, screenHeight);

                mpf_t realInterpolate;
                mpf_init(realInterpolate);
                interpolateSpan(minReal, realSpan, widthProgress, realInterpolate);

                mpf_t imaginaryInterpolate;
                mpf_init(imaginaryInterpolate);
                interpolateSpan(minImaginary, imaginarySpan, heightProgress, imaginaryInterpolate);
                ImaginaryNumber pixel(realInterpolate, imaginaryInterpolate);
                bool bound = isMandlebrotBound(pixel, mandlebrotIterations);
                screen[y * nScreenWidth + x] = bound ? 'A' : ' ';

                mpf_clear(widthProgress);
                mpf_clear(heightProgress);
                mpf_clear(realInterpolate);
                mpf_clear(imaginaryInterpolate);
            }
        }
        sectionsComputed++;
    }
}

void addSectionToCompute(int minX, int maxX, int minY, int maxY)
{
    {
        unique_lock<mutex> lock(mtx);
        Queue.push(make_tuple(minX, maxX, minY, maxY));
    }
    cv.notify_one();
}

int main()
{
    int numThreads = thread::hardware_concurrency();
    vector<thread> threadPool;
    for (int ii = 0; ii < numThreads; ii++)
    {
        threadPool.push_back(thread(computeMandlebrotSection));
    }
    logger.open("log.txt");
    WINDOW *currWin = initscr();
    getmaxyx(currWin, nScreenHeight, nScreenWidth);
    mpf_init_set_d(screenWidth, nScreenWidth);
    mpf_init_set_d(screenHeight, nScreenHeight);

    mpf_init_set_si(defaultMinReal, -2);
    mpf_init_set_si(defaultMaxReal, 1);

    mpf_init_set_si(defaultMinImaginary, 1);
    mpf_init_set_si(defaultMaxImaginary, -1);

    screen = new char[nScreenWidth * nScreenHeight];
    mpf_init_set(minReal, defaultMinReal);
    mpf_init_set(maxReal, defaultMaxReal);
    mpf_init_set(minImaginary, defaultMinImaginary);
    mpf_init_set(maxImaginary, defaultMaxImaginary);

    mpf_init(movement);
    mpf_init(realSpan);
    mpf_init(imaginarySpan);

    while (1)
    {
        mpf_sub(realSpan, maxReal, minReal);
        mpf_sub(imaginarySpan, maxImaginary, minImaginary);
        for (int x = 0; x < nScreenWidth; x++)
        {
            addSectionToCompute(x, x + 1, 0, nScreenHeight);
        }
        totalSectionsToCompute += nScreenWidth;
        while (sectionsComputed < totalSectionsToCompute)
        {
        }
        clear();
        mvaddstr(0, 0, screen);
        refresh();

        char input = getch();

        switch (input)
        {
        case 'z': // RESOLUTION +
            resolution += resolutionStep;
            break;
        case 'x': // RESOLUTION -
            resolution -= resolutionStep;
            break;
        case 'w': // UP
        {
            mpf_t coefficient;
            mpf_init_set_d(coefficient, 0.05);
            mpf_mul(movement, coefficient, imaginarySpan);
            mpf_sub(minImaginary, minImaginary, movement);
            mpf_sub(maxImaginary, maxImaginary, movement);
            mpf_clear(coefficient);
        }
        break;
        case 's': // DOWN
        {
            mpf_t coefficient;
            mpf_init_set_d(coefficient, 0.05);
            mpf_mul(movement, coefficient, imaginarySpan);
            mpf_add(minImaginary, minImaginary, movement);
            mpf_add(maxImaginary, maxImaginary, movement);
            mpf_clear(coefficient);
        }

        break;
        case 'd': // RIGHT
        {
            mpf_t coefficient;
            mpf_init_set_d(coefficient, 0.05);
            mpf_mul(movement, coefficient, realSpan);
            mpf_add(minReal, minReal, movement);
            mpf_add(maxReal, maxReal, movement);
            mpf_clear(coefficient);
        }
        break;
        case 'a': // LEFT
        {
            mpf_t coefficient;
            mpf_init_set_d(coefficient, 0.05);
            mpf_mul(movement, coefficient, realSpan);
            mpf_sub(minReal, minReal, movement);
            mpf_sub(maxReal, maxReal, movement);
            mpf_clear(coefficient);
        }
        break;
        case 'q': // ZOOM+
        {
            mpf_t half;
            mpf_init_set_d(half, 0.5);

            mpf_t realCenter;
            mpf_init(realCenter);
            interpolateSpan(minReal, realSpan, half, realCenter);

            mpf_t imaginaryCenter;
            mpf_init(imaginaryCenter);
            interpolateSpan(minImaginary, imaginarySpan, half, imaginaryCenter);

            Vector2D center(realCenter, imaginaryCenter);

            Vector2D topLeft(minReal, minImaginary);
            Vector2D bottomRight(maxReal, maxImaginary);

            Vector2D *newTopLeft = topLeft.Subtract(center)->Multiply(0.5)->Add(center);
            Vector2D *newBottomRight = bottomRight.Subtract(center)->Multiply(0.5)->Add(center);

            newTopLeft->X(minReal);
            newTopLeft->Y(minImaginary);

            newBottomRight->X(maxReal);
            newBottomRight->Y(maxImaginary);

            mpf_clear(half);
            mpf_clear(realCenter);
            mpf_clear(imaginaryCenter);
            zoomFactor++;
        }
        break;
        case 'e': // ZOOM-
        {
            mpf_t half;
            mpf_init_set_d(half, 0.5);

            mpf_t realCenter;
            mpf_init(realCenter);
            interpolateSpan(minReal, realSpan, half, realCenter);

            mpf_t imaginaryCenter;
            mpf_init(imaginaryCenter);
            interpolateSpan(minImaginary, imaginarySpan, half, imaginaryCenter);

            Vector2D center(realCenter, imaginaryCenter);

            Vector2D topLeft(minReal, minImaginary);
            Vector2D bottomRight(maxReal, maxImaginary);

            Vector2D *newTopLeft = topLeft.Subtract(center)->Multiply(2)->Add(center);
            Vector2D *newBottomRight = bottomRight.Subtract(center)->Multiply(2)->Add(center);

            newTopLeft->X(minReal);
            newTopLeft->Y(minImaginary);

            newBottomRight->X(maxReal);
            newBottomRight->Y(maxImaginary);

            mpf_clear(half);
            mpf_clear(realCenter);
            mpf_clear(imaginaryCenter);
            zoomFactor--;
        }
        break;
        }
        logger.flush();
    }
    logger.close();
    endwin();
    return 0;
}