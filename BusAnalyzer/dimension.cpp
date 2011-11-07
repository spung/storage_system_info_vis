#include "dimension.h"

Dimension::Dimension(int id, QString title, bool vis, Equation *eq){
    this->id = id;
    this->title = title;
    this->visible = vis;
    this->scaleEq = eq;
}

double Dimension::getValue(int mode, double value){
    if(mode == MODE_OVERVIEW){
        return this->scaleEq->getValue(this->min, this->max, value);
    }
    else if(mode == MODE_FOCUS){
        return this->scaleEq->getValue(this->currentMin, this->currentMax, value);
    }
}
