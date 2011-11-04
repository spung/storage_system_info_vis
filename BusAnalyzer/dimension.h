#ifndef DIMENSION_H
#define DIMENSION_H
#include "equation.h"
#include <string>
#include <stdlib.h>
#include <QMainWindow>


using namespace std;

typedef struct label {
   QString name;
   double value;
} label;

class Dimension
{
public:
    double min, max, difference, currentMin, currentMax;
    Equation *scaleEq;
    QString title;
    bool visible;
    label *labels[10];
    int id;

    Dimension(int id, QString title, bool vis, Equation *eq);

    double getValue(double value);
};



#endif // DIMENSION_H
