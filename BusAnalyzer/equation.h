/**
    Author: Steven Pungdumri
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/

#ifndef EQUATION_H
#define EQUATION_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <QString>

// equation class is used to display dimensions on different scales.
//  this allows further development in other equations to use for displaying dimensions
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
