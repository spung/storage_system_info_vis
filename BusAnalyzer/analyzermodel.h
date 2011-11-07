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

#define MODE_OVERVIEW             0
#define MODE_FOCUS                1
#define MODE_FOCUS_THREADS        2

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

    QString error;
    QVector<Record*> records;
    QVector<Dimension*>  dimensions;
    QVector<int> order;
    QHash<QString, int> hidden;

    short mode;
    Dimension* focus_dimension;
    double focus_min, focus_max;
};

#endif // ANALYZERMODEL_H
