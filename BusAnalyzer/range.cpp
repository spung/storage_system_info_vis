/**
    Author: Steven Pungdumri
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/

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
