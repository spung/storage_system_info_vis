/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
/**
    Author: Steven Pungdumri (additions to the nokia template)
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/
#include <QtGui>
#include <QtOpenGL>

#include "mainwindow.h"

// initializations of window components
MainWindow::MainWindow()
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    statusBar()->showMessage(tr(("Ready")));

    glWidget = new GLObject();

    pixmapLabel = new QLabel;

    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    glWidgetArea->setWidgetResizable(true);

    glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    glWidgetArea->setMinimumSize(50, 50);

    pixmapLabelArea = new QScrollArea;
    pixmapLabelArea->setWidget(pixmapLabel);
    pixmapLabelArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    pixmapLabelArea->setMinimumSize(50, 50);

    xSlider = createSlider(SIGNAL(xRotationChanged(int)),
                           SLOT(setXRotation(int)));
    renderCount = 0;
    createActions();
    createMenus();


    QBoxLayout *centralLayout = new QBoxLayout(QBoxLayout::TopToBottom, centralWidget);
    centralLayout->addWidget(glWidgetArea);
    centralLayout->addWidget(xSlider);
    centralWidget->setLayout(centralLayout);

    glWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(glWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ctxMenu(const QPoint &)));

    connect(glWidget, SIGNAL(valueChanged()), this, SLOT(refresh()));
    connect(glWidget, SIGNAL(doneRendering()), this, SLOT(finishedDrawing()));

    setWindowTitle(tr("Storage System Workload Visualization"));
    resize(1024, 600);

    model = NULL;
}

// display a context menu to the user when right-clicking on the visualization
void MainWindow::ctxMenu(const QPoint &pos){
    if(model != NULL){

        float section = glWidget->width()/(2*model->order.size() - 1);
        int clickedSection = (pos.x())/section;

        this->pointClicked = pos;
        this->sectionClicked = clickedSection;

        if(clickedSection % 2 == 0){
            dimensionClick(pos);
        }
        else{
            marginClick(pos);
        }
    }
}

// applies a linear scale to a dimension by invoking this action in the model
void MainWindow::setLinear(){
    model->setLinear(this->sectionClicked/2);
    qDebug() << QString("converting %1 to linear...").arg(QString::number(this->sectionClicked/2));
    this->refresh();
}

// applies a logarithmic scale to a dimension by invoking this action in the model
void MainWindow::setLog(){
    model->setLog(this->sectionClicked/2);
    this->refresh();
}

// hides a dimension by invoking this action in the model
void MainWindow::hideDim(){
    model->hideAt(this->sectionClicked/2);
    this->refresh();
}

// handles clicks between axes, prompts the user to insert a hidden dimension
void MainWindow::marginClick(const QPoint &pos){
    QMenu *menu = new QMenu;
    QMenu *insertMenu = menu->addMenu(tr("Insert..."));

    QHashIterator<QString, int> iter(model->hidden);
    while(iter.hasNext()){
        insertMenu->addAction(tr(qPrintable(iter.next().key())), this, SLOT(insertDim()) );
    }
    menu->exec(glWidget->mapToGlobal(pos));
}

// inserts and shows a previously hidden dimension
void MainWindow::insertDim(){
    QAction *sender = (QAction*) this->sender();
    model->insertAt(sectionClicked-sectionClicked/2, &(sender->text()));
    this->refresh();
}

// set the overview mode
void MainWindow::setOverviewMode(){
    model->mode = MODE_OVERVIEW;
    overviewModeAct->setDisabled(true);
    focusModeAct->setEnabled(true);
    this->refresh();
}

// set the focus mode
void MainWindow::setFocusMode(){
    model->mode = MODE_FOCUS;
    focusModeAct->setDisabled(true);
    overviewModeAct->setEnabled(true);
    this->refresh();
}

// set the color thread mode
void MainWindow::setThreadMode(){
    if(model->colorThreads == false){
        model->colorThreads = true;
        threadModeAct->setText(QString("Disable Color Threads"));
    }
    else{
        model->colorThreads = false;
        threadModeAct->setText(QString("Enable Color Threads"));
    }
    this->refresh();
}

// initiate process to apply a focus
void MainWindow::focus(){
    getRange(ACTION_FOCUS);
}

// initiate process to apply a brush
void MainWindow::brush(){
    getRange(ACTION_BRUSH);
}

// set the min field to the parameter value
void MainWindow::setMinField(int value){
    currentMin->setText(QString::number(value));
}

// set the max field to the parameter value
void MainWindow::setMaxField(int value){
    currentMax->setText(QString::number(value));
}

// set the min slider to the parameter value
void MainWindow::setMinSlider(QString value){
    bool ok;
    int intVal = value.toInt(&ok, 10);
    if(ok)
        minSlider->setValue(intVal);
}

// set the max slider to the parameter value
void MainWindow::setMaxSlider(QString value){
    bool ok;
    int intVal = value.toInt(&ok, 10);
    if(ok)
        maxSlider->setValue(intVal);
}

// displays a slider to the user to select a range
void MainWindow::getRange(short action){
    Dimension *currentDim = model->dimensions.at(model->order.at(sectionClicked/2));
    Range *range = new Range(this);

    qFindChild<QLabel*>(range, "title")->setText(currentDim->title);
    qFindChild<QLabel*>(range, "minMin")->setText(QString("%1").arg(currentDim->min, 0, 'g', 10));
    qFindChild<QLabel*>(range, "minMax")->setText(QString("%1").arg(currentDim->max, 0, 'g', 10));
    qFindChild<QLabel*>(range, "maxMin")->setText(QString("%1").arg(currentDim->min, 0, 'g', 10));
    qFindChild<QLabel*>(range, "maxMax")->setText(QString("%1").arg(currentDim->max, 0, 'g', 10));

    minSlider = qFindChild<QSlider*>(range, "minSlider");
    minSlider->setMinimum(floor(currentDim->min));
    minSlider->setMaximum(ceil(currentDim->max));

    maxSlider = qFindChild<QSlider*>(range, "maxSlider");
    maxSlider->setMinimum(floor(currentDim->min));
    maxSlider->setMaximum(ceil(currentDim->max));

    currentMin = qFindChild<QLineEdit*>(range, "currentMinValue");
    currentMin->setText(QString::number(minSlider->value()));
    connect( minSlider, SIGNAL(sliderMoved(int)), this, SLOT(setMinField(int)) );
    connect(currentMin, SIGNAL(textChanged(QString)), this, SLOT(setMinSlider(QString)));

    currentMax = qFindChild<QLineEdit*>(range, "currentMaxValue");
    currentMax->setText(QString::number(ceil(currentDim->max)));
    connect( maxSlider, SIGNAL(sliderMoved(int)), this, SLOT(setMaxField(int)) );
    connect(currentMax, SIGNAL(textChanged(QString)), this, SLOT(setMaxSlider(QString)));

    minSlider->setValue(currentDim->getCurrentMin());
    currentMin->setText(QString::number(currentDim->getCurrentMin()));
    maxSlider->setValue(currentDim->getCurrentMax());
    currentMax->setText(QString::number(currentDim->getCurrentMax()));

    switch(action){
        case ACTION_BRUSH:
            range->setWindowTitle("Select Brush Range");
            break;
        case ACTION_FOCUS:
            range->setWindowTitle("Select Focus Range");            
            break;
        default:
            break;
    }

    if(range->exec() == Range::Accepted){
        if(minSlider->value() > maxSlider->value()){
            QMessageBox::information(this, tr("Invalid Input"), tr("Minimum value must be less than maximum value."));
        }
        else{
            switch (action){
                case ACTION_BRUSH:
                    model->setBrushCriteria(model->order.at(sectionClicked/2), minSlider->value(), maxSlider->value());
                    break;
                case ACTION_FOCUS:
                    model->setFocus(currentDim, minSlider->value(), maxSlider->value());
                    setFocusMode();
                    break;
                default:
                    break;
            }
            this->refresh();
        }
    }
}

// handles clicks and passes them on to the model for manipulating axes
void MainWindow::dimensionClick(const QPoint &pos){
    QMenu *menu = new QMenu;
    menu->addAction(tr("Remove Dimension"), this, SLOT(hideDim()));

    menu->addAction(tr("Brush range..."), this, SLOT(brush()));

    menu->addAction(tr("Create Focus"), this, SLOT(focus()));

    QMenu *equationMenu = menu->addMenu(tr("Set Equation"));

    if(model->dimensions.at(model->order.at(sectionClicked/2))->scaleEq->getType().compare("logten") == 0){
        equationMenu->addAction(tr("Linear"), this, SLOT(setLinear()));
    }
    else{
        equationMenu->addAction(tr("Logarithmic"), this, SLOT(setLog()));
    }
    menu->exec(glWidget->mapToGlobal(pos));
}

// reset transforms
void MainWindow::reset(){
    glWidget->reset();
    xSlider->setValue(0);
    refresh();
}

// notify the user in the status bar that drawing is finished.
void MainWindow::finishedDrawing(){
    statusBar()->showMessage(tr("Finished."), 2000);
}

// show open file dialog to user
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Select a CSV file to open",
                QString("."),
                " (*.csv)");
    qDebug() << fileName;
    if (!fileName.isEmpty()){
        loadFile(fileName);
        this->refresh();
    }
}

// handles window resizes
void MainWindow::resizeEvent(QResizeEvent *e)
{
  // call the base method
  QMainWindow::resizeEvent(e);
    glWidget->refreshTranslations();
}

//initialize and set the openGL object
void MainWindow::setGL()
{
    glWidget = new GLObject;

    connect(glWidget, SIGNAL(valueChanged()),
            this, SLOT(setPixelmap()) );

    glWidgetArea->setWidget(glWidget);
    glWidget->model = this->model;
    this->refresh();
}

// about menu
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Storage System Workload Visualization"),
            tr("Steven Pungdumri's Thesis:\n\nThis tool provides an interactive experience in visualizing workload data, allowing manipulation of the graph by reordering axes, concealing axes, highlighting particular ranges of data, scaling particular areas of interest, and viewing the machine suggested areas of interest. \n\nIt also assigns unique colors to identified threads and renders this when enabled. \n\nWork conducted with Western Digital Corporation."));
}

// create and assign actions to each of the menu items
void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    resetAct = new QAction(tr("&Reset Transforms"), this);
    resetAct->setShortcut(tr("Ctrl+R"));
    connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));
    resetAct->setDisabled(true);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    overviewModeAct = new QAction(tr("&Overview"), this);
    connect(overviewModeAct, SIGNAL(triggered()), this, SLOT(setOverviewMode()));
    overviewModeAct->setDisabled(true);

    focusModeAct = new QAction(tr("&Focus"), this);
    connect(focusModeAct, SIGNAL(triggered()), this, SLOT(setFocusMode()));
    focusModeAct->setDisabled(true);

    threadModeAct = new QAction(tr("&Enable Color Threads"), this);
    connect(threadModeAct, SIGNAL(triggered()), this, SLOT(setThreadMode()));
    threadModeAct->setDisabled(true);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

// refresh the rendering to show changes in the model
void MainWindow::refresh(){
    statusBar()->showMessage(tr("Rendering..."), 2000);
    glWidgetArea->hide();
    glWidgetArea->show();    
}

// creates the top menus in the application
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(resetAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(overviewModeAct);
    modeMenu->addAction(focusModeAct);
    modeMenu->addSeparator();
    modeMenu->addAction(threadModeAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

// creates a slider to take a range input from the user
QSlider *MainWindow::createSlider(const char *changedSignal,
                                  const char *setterSlot)
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 10);
    slider->setSingleStep(1);
    slider->setPageStep(2);
    slider->setTickInterval(1);
    slider->setTickPosition(QSlider::TicksRight);
    connect(slider, SIGNAL(valueChanged(int)), glWidget, setterSlot);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(refresh()));
    return slider;
}

// load a dataset from a file to the model
void MainWindow::loadFile(const QString &fileName)
{
    model = new AnalyzerModel();
    if(!model->loadFile(fileName)){
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(model->error));
        return;
    }

    glWidget->model = model;

    this->refresh();
    statusBar()->showMessage(tr("File loaded"), 2000);

    resetAct->setEnabled(true);
    threadModeAct->setEnabled(true);
}
