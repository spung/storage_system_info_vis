/**
    Author: Steven Pungdumri
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/

#ifndef ANALYZERMODEL_H
#define ANALYZERMODEL_H

#include "dimension.h"
#include <QFile>
#include <QStack>
#include <QColor>
#include <time.h>
#include "record.h"
#include <QVector>
#include <QHash>
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <float.h>

// constants for each mode
#define MODE_OVERVIEW           0
#define MODE_FOCUS              1

// constants for each dimension
#define START_TIME              0
#define END_TIME                1
#define START_ID                2
#define END_ID                  3
#define QUEUEABLE               4
#define CMD                     5
#define INTER_CMD_TIME          6
#define LBA                     7
#define LENGTH                  8
#define ALIGNMENT               9
#define FUA                     10
#define CCT                     11
#define QUEUE_CCT               12
#define QUEUE_DEPTH             13
#define END_QUEUE_DEPTH         14
#define FIFO_POS                15
#define SEQUENTIAL              16
#define STREAM_NUM              17
#define CACHE_HIT               18

// the primary model for the tool, containing records, dimensions, and tool modes
class AnalyzerModel
{
public:
    AnalyzerModel();

    // load a dataset from a file
    bool loadFile(const QString &fileName);

    // print each record (debugging)
    void printRecords();

    // set a particular dimension to hidden
    void hideAt(int position);

    // set a particular dimension to display a logarithmic scale
    void setLog(int position);

    // set a particular dimension to display a linear scale
    void setLinear(int position);

    // insert a hidden dimension at a particular position
    void insertAt(int position, QString *text);

    // set the global brush range for a particular dimension and range
    void setBrushCriteria(int dimension, double min, double max);

    // set the global focus for a particular dimension and range
    void setFocus(Dimension*dim, double min, double max);

    // check if a particular record is shown in the current focus
    bool inFocus(Record*rec);

    // returns true if the dimension is discrete, false otherwise
    bool isEnumDim(int dim);

    // generate the colors for each thread
    void generateColorThreads();

    // initialize the visualization for a particular order (hardcoded)
    void initOrder();

    // returns the dimension the brushing is selected on
    Dimension* getBrushDimension();

    // returns the dimension the focus is selected on
    Dimension* getFocusDimension();

    // returns the current brush max value
    double getBrushMax();

    // returns the current brush min value
    double getBrushMin();

    // returns the current focus max value
    double getFocusMax();

    // returns the current focus min value
    double getFocusMin();

    // returns the frequency count at a particular dimension/position
    double getHistogramValue(int id, int position);

    // resets the focus counts
    void resetFocusCounts();

    QString error;

    // holds all of the records of the dataset
    QVector<Record*> records;

    // holds all of the dimensions of the dataset
    QVector<Dimension*>  dimensions;

    // the order which the dimensions are currently displayed
    QVector<int> order;

    // dimesnions currently hidden
    QHash<QString, int> hidden;

    // the current mode of the visualization (overview or focus)
    short mode;

    // pointer to the dimension the focus is applied on
    Dimension* focus_dimension;

    // pointer to the dimension the brushing is applied on
    Dimension* brush_dimension;

    // global values for focus and brush min/max
    double focus_min, focus_max, brush_min, brush_max;

    // determines whether threads should be colored or rendered in black
    bool colorThreads;

    // the count of records displayed with a focus applied
    int focusCount;
};

#endif // ANALYZERMODEL_H
