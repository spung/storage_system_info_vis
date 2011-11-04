#include "equation.h"
#include <QDebug>
#include <QString>

Equation::Equation(){}
double Equation::getValue(double min, double max, double value){
    return (value-min)/(max-min);
}
QString Equation::getType(){ return QString("equation");}

Linear::Linear(){}
double Linear::getValue(double min, double max, double value){
    return (value-min)/(max-min);
}

QString Linear::getType(){ return QString("linear");}

LogTen::LogTen(){}
double LogTen::getValue(double min, double max, double value){
    if(value == 0 || value <= min) return min;
    else
        return log10(10000*value-min)/log10(10000*max-min);
}

QString LogTen::getType(){ return QString("logten");}
