/**
    Author: Steven Pungdumri
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/

#include "dimension.h"
#include <QDebug>

// dimension constructor
Dimension::Dimension(int id, QString title, bool vis, Equation *eq){
    this->id = id;
    this->title = title;
    this->visible = vis;
    this->scaleEq = eq;
}

// returns the value where the coordinate should be drawn on the axis
double Dimension::getValue(int mode, double value){
    // for overview mode, use the global min/max of this dimension
    if(mode == MODE_OVERVIEW){
        return this->scaleEq->getValue(this->min, this->max, value);
    }
    // for focus mode, use the current focus' min/max of this dimension
    else if(mode == MODE_FOCUS){
        return this->scaleEq->getValue(this->currentMin, this->currentMax, value);
    }
}

// return the current min value of the focus
double Dimension::getCurrentMin(){
    return currentMin;
}

// return the current max value of the focus
double Dimension::getCurrentMax(){
    return currentMax;
}

// discrete dimension constructor
Discrete::Discrete(int id, QString title, bool vis, Equation *eq) : Dimension(id, title, vis, eq){
    this->id = id;
    this->title = title;
    this->visible = vis;
    this->scaleEq = eq;
}

// initializes all focus counts for this dimension
void Discrete::resetFocusCounts(){
    for(int i=0; i<focusCount.size(); i++){
        focusCount.replace(i, 0);
    }
}

// insert a new name/value (for labeling)
void Discrete::insertNameValue(QString *name){
    nameValues.append(*name);
}

// return the size of the name/values collection (for labeling)
int Discrete::getNameValuesSize(){
    return nameValues.size();
}

// return the name at a particular discrete value (for labeling)
QString Discrete::getNameValueAt(int position){
    return nameValues.at(position);
}

// initialize the counts (for histograms)
void Discrete::initCount(int numValues){
    for(int i = 0; i < numValues; i++){
        count.append(0);
        focusCount.append(0);
    }
}

// return the global count at a particluar discrete value
int Discrete::getCount(int position){
    return count.at(position);
}

// return the focus count at a particular discrete value
int Discrete::getFocusCount(int position){
    return focusCount.at(position);
}

// increment the global count at a certain position
void Discrete::incrementCount(int position){
    int current = count.at(position);
    count.replace(position, current + 1);
}

//increment the focus count at a certain discrete position
void Discrete::incrementFocusCount(int position){
    int current = focusCount.at(position);
    focusCount.replace(position, current + 1);
}

// return the position value of a discrete data value (for labeling)
double Discrete::getValue(int mode, double value){
    // currently there is no differentiation between overview and focus modes
    if(mode == MODE_OVERVIEW){
        return this->scaleEq->getValue(0, nameValues.size()-1, value);
    }
    else if(mode == MODE_FOCUS){
        return this->scaleEq->getValue(0, nameValues.size()-1, value);
    }
}

// constructor for continuous dimensions
Continuous::Continuous(int id, QString title, bool vis, Equation *eq) : Dimension(id, title, vis, eq){
    this->id = id;
    this->title = title;
    this->visible = vis;
    this->scaleEq = eq;
}
