#include "dimension.h"
#include <QDebug>

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

double Dimension::getCurrentMin(){
    return currentMin;
}

double Dimension::getCurrentMax(){
    return currentMax;
}

Discrete::Discrete(int id, QString title, bool vis, Equation *eq) : Dimension(id, title, vis, eq){
    this->id = id;
    this->title = title;
    this->visible = vis;
    this->scaleEq = eq;
}

void Discrete::insertNameValue(QString *name){
    nameValues.append(*name);
}

int Discrete::getNameValuesSize(){
    return nameValues.size();
}

QString Discrete::getNameValueAt(int position){
    return nameValues.at(position);
}

void Discrete::initCount(int numValues){
    for(int i = 0; i < numValues; i++){
        count.append(0);
        focusCount.append(0);
    }
}

int Discrete::getCount(int position){
    return count.at(position);
}

int Discrete::getFocusCount(int position){
    return focusCount.at(position);
}

void Discrete::incrementCount(int position){
    int current = count.at(position);
    count.replace(position, current + 1);
}

void Discrete::incrementFocusCount(int position){
    int current = focusCount.at(position);
    focusCount.replace(position, current + 1);
}

double Discrete::getValue(int mode, double value){
    if(mode == MODE_OVERVIEW){
        return this->scaleEq->getValue(0, nameValues.size()-1, value);
    }
    else if(mode == MODE_FOCUS){
        return this->scaleEq->getValue(0, nameValues.size()-1, value);
    }
    qDebug()<< QString("got here discrete");
}

Continuous::Continuous(int id, QString title, bool vis, Equation *eq) : Dimension(id, title, vis, eq){
    this->id = id;
    this->title = title;
    this->visible = vis;
    this->scaleEq = eq;
}
