#ifndef DIMENSION_H
#define DIMENSION_H
#include "equation.h"
#include <string>
#include <stdlib.h>
#include <QMainWindow>

#define MODE_OVERVIEW           0
#define MODE_FOCUS              1

using namespace std;

// a label for a dimension consists of a string name, and a value where it should appear on the axis
typedef struct label {
   QString name;
   double value;
} label;

class Dimension
{
    public:
        Dimension(int id, QString title, bool vis, Equation *eq);

        /*
            min - minimum value in this dataset
            max - maximum value in this dataset
            difference - max - min
            currentMin - current minimum value in the focus applied
            currentMax - current maximum value in the focus applied
        */
        double min, max, difference, currentMin, currentMax;

        // the equation applied to this dimension (right now linear or logarithmic
        Equation *scaleEq;

        // title label of this dimension
        QString title;

        // whether the user has hidden this dimension or not
        bool visible;

        // each dimension has 10 labels (arbitrary), mainly used for discrete dimensions currently
        label *labels[10];

        // the id of the dimension (used for getting the value from records
        int id;

        virtual double getValue(int mode, double value);
        double getCurrentMin();
        double getCurrentMax();
        virtual int getNameValuesSize(){return 0;}
        virtual void incrementCount(int position){}
        virtual void incrementFocusCount(int position){}
        virtual int getCount(int position){return -1;}
        virtual int getFocusCount(int position){return -1;}
        virtual void resetFocusCounts(){}
};

// dimension with continuous values
class Continuous: public Dimension
{
    public:
        Continuous(int id, QString title, bool vis, Equation *eq);

        // not relevant for continuous dimensions
        int getNameValuesSize(){return 0;}
};

// dimension with discrete values
class Discrete: public Dimension
{
    public:
        Discrete(int id, QString title, bool vis, Equation *eq);

        // each discrete dimension has name values for the string representation
        //  of discrete values
        QVector<QString> nameValues;

        // a count of records at a particular discrete value (for histograms)
        QVector<int> count;

        // a count of records at a particular discrete value (for histograms)
        //  when a focus is set
        QVector<int> focusCount;

        // return the position value corresponding to the data value
        double getValue(int mode, double value);

        // initialize the count for the discrete dimension
        void initCount(int numValues);

        // return the count for a particular discrete position (for histograms)
        int getCount(int position);

        // return the count within a focus for a particular discrete position (for histograms)
        int getFocusCount(int position);

        // initialize the focus counts prior to generating them when another focus is generated
        void resetFocusCounts();

        // increments the count of records at a particular discrete value
        // (for histograms)
        void incrementCount(int position);

        // increments the count of records at a particular discrete value
        // (for histograms) for a particular focus applied
        void incrementFocusCount(int position);

        // inserts a name/value, used in parsing/initilization of dimensions (for labeling)
        void insertNameValue(QString *name);

        // return the number of name value pairs (for labeling)
        int getNameValuesSize();

        // returns the name at a discrete position (for labeling)
        QString getNameValueAt(int position);
};

#endif // DIMENSION_H
