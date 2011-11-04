#ifndef RANGE_H
#define RANGE_H

#include <QDialog>

namespace Ui {
    class Range;
}

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
