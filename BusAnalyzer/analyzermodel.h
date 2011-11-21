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

#define MODE_OVERVIEW           0
#define MODE_FOCUS              1

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

class AnalyzerModel
{
public:
    AnalyzerModel();
    bool loadFile(const QString &fileName);
    void printRecords();
    void hideAt(int position);
    void setLog(int position);
    void setLinear(int position);
    void insertAt(int position, QString *text);
    void setBrushCriteria(int dimension, double min, double max);
    void setFocus(Dimension*dim, double min, double max);
    bool inFocus(Record*rec);
    bool isEnumDim(int dim);
    void generateColorThreads();
    void initOrder();
    Dimension* getBrushDimension();
    Dimension* getFocusDimension();
    double getBrushMax();
    double getBrushMin();
    double getFocusMax();
    double getFocusMin();
    double getHistogramValue(int id, int position);
    void resetFocusCounts();

    QString error;
    QVector<Record*> records;
    QVector<Dimension*>  dimensions;
    QVector<int> order;
    QHash<QString, int> hidden;

    short mode;
    Dimension* focus_dimension;
    Dimension* brush_dimension;
    double focus_min, focus_max, brush_min, brush_max;
    bool colorThreads;
    int focusCount;
};

#endif // ANALYZERMODEL_H
