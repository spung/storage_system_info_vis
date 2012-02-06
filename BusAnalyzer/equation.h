#ifndef EQUATION_H
#define EQUATION_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <QString>

class Equation
{
    public:
        Equation();

        // Computes the value with the current set equation
        virtual double getValue(double min, double max, double value);

        // Returns the type of equation currently set
        virtual QString getType();
};

// Linear equation
class Linear: public Equation
{
    public:
        Linear();
        virtual double getValue(double min, double max, double value);
        virtual QString getType();
};

// Logarithmic equation
class LogTen: public Equation
{
    public:
        LogTen();
        virtual double getValue(double min, double max, double value);
        virtual QString getType();
};

#endif // EQUATION_H
