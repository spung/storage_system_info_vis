#include "dimension.h"

Dimension::Dimension(int id, QString title, bool vis, Equation *eq){
    this->id = id;
    this->title = title;
    this->visible = vis;
    this->scaleEq = eq;
}

double Dimension::getValue(double value){
    return this->scaleEq->getValue(this->currentMin, this->currentMax, value);
}
