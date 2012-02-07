/**
    Author: Steven Pungdumri
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/

#ifndef RANGE_H
#define RANGE_H

#include <QDialog>

namespace Ui {
    class Range;
}

// a dialog class for selecting ranges of data (brushing, focus)
//  the rest is designed in range.ui
class Range : public QDialog
{
    Q_OBJECT

public:
    explicit Range(QWidget *parent = 0);
    ~Range();

private:
    Ui::Range *ui;

};

#endif // RANGE_H
