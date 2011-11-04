#include "range.h"
#include "ui_range.h"

Range::Range(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Range)
{
    ui->setupUi(this);
}

Range::~Range()
{
    delete ui;
}
