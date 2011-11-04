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
        virtual double getValue(double min, double max, double value);
        virtual QString getType();
};

class Linear: public Equation
{
    public:
        Linear();
        virtual double getValue(double min, double max, double value);
        virtual QString getType();
};

class LogTen: public Equation
{
    public:
        LogTen();
        virtual double getValue(double min, double max, double value);
        virtual QString getType();
};

#endif // EQUATION_H
