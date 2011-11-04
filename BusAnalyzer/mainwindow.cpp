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

#include <QtGui>
#include <QtOpenGL>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    statusBar()->showMessage(tr(("Ready")));

    glWidget = new GLObject();
//    connect(glWidget, SIGNAL(valueChanged()),
//            this, SLOT(setPixelmap()) );
    pixmapLabel = new QLabel;

    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    glWidgetArea->setWidgetResizable(true);
//    glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

    setWindowTitle(tr("Bus Analyzer"));
    resize(1024, 600);

    model = NULL;
}

void MainWindow::ctxMenu(const QPoint &pos){
    qDebug() << "clicked";
    if(model != NULL){

        //float section = pixmapLabel->width()/(2*model->order.size() - 1);
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
//    qDebug() << QString("Position: x: %1, y: %2").arg(QString::number(pos.x())).arg(QString::number(pos.y()));
//    qDebug()<< QString("width: %1").arg(QString::number(glWidget->width()));
//    qDebug() << QString("Clicked on: %1").arg(QString::number(clickedSection));
}

void MainWindow::setLinear(){
    model->setLinear(this->sectionClicked/2);
    qDebug() << QString("converting %1 to linear...").arg(QString::number(this->sectionClicked/2));
    //this->setGL();
    this->refresh();
}

void MainWindow::setLog(){
    model->setLog(this->sectionClicked/2);
    qDebug() << QString("converting %1 to log...").arg(QString::number(this->sectionClicked/2));
    //this->setGL();
    this->refresh();
}

void MainWindow::hideDim(){
    model->hideAt(this->sectionClicked/2);
    qDebug() << QString("hiding %1...").arg(QString::number(this->sectionClicked/2));
    //this->setGL();
    this->refresh();
}

void MainWindow::marginClick(const QPoint &pos){
    QMenu *menu = new QMenu;
    QMenu *insertMenu = menu->addMenu(tr("Insert..."));

    QHashIterator<QString, int> iter(model->hidden);
    while(iter.hasNext()){
        insertMenu->addAction(tr(qPrintable(iter.next().key())), this, SLOT(insertDim()) );
    }
   // menu->exec(pixmapLabel->mapToGlobal(pos));
    menu->exec(glWidget->mapToGlobal(pos));
}

void MainWindow::insertDim(){
    QAction *sender = (QAction*) this->sender();
    qDebug()<< sender->text();
    model->insertAt(sectionClicked-sectionClicked/2, &(sender->text()));
    qDebug() << QString("inserting at %1...").arg(QString::number(this->sectionClicked/2));
    //this->setGL();
    this->refresh();
}

void MainWindow::focusTest(){
    model->setFocus(model->focus_dimension, model->focus_min, model->focus_max);
    this->refresh();
    //getRange(ACTION_FOCUS);
}

void MainWindow::focus(){
    getRange(ACTION_FOCUS);
}

void MainWindow::brush(){
    getRange(ACTION_BRUSH);
}

void MainWindow::getRange(short action){
    Dimension *currentDim = model->dimensions.at(model->order.at(sectionClicked/2));
    Range *range = new Range(this);
    range->setWindowTitle("Select Brush Range");

    qFindChild<QLabel*>(range, "title")->setText(currentDim->title);
    qFindChild<QLabel*>(range, "minMin")->setText(QString("%1").arg(currentDim->min, 0, 'g', 10));
    qFindChild<QLabel*>(range, "minMax")->setText(QString("%1").arg(currentDim->max, 0, 'g', 10));
    qFindChild<QLabel*>(range, "maxMin")->setText(QString("%1").arg(currentDim->min, 0, 'g', 10));
    qFindChild<QLabel*>(range, "maxMax")->setText(QString("%1").arg(currentDim->max, 0, 'g', 10));

    QSlider *minSlider = qFindChild<QSlider*>(range, "minSlider");
    minSlider->setMinimum(floor(currentDim->min));
    minSlider->setMaximum(ceil(currentDim->max));

    QSlider *maxSlider = qFindChild<QSlider*>(range, "maxSlider");
    maxSlider->setMinimum(floor(currentDim->min));
    maxSlider->setMaximum(ceil(currentDim->max));

    QLabel *currentMin = qFindChild<QLabel*>(range, "currentMinValue");
    currentMin->setText(QString::number(minSlider->value()));
    connect(minSlider, SIGNAL(valueChanged(int)), currentMin, SLOT(setNum(int)));

    QLabel *currentMax = qFindChild<QLabel*>(range, "currentMaxValue");
    currentMax->setText(QString::number(maxSlider->value()));
    connect(maxSlider, SIGNAL(valueChanged(int)), currentMax, SLOT(setNum(int)));

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
                    break;
                default:
                    break;
            }
            this->refresh();
        }
    }
}

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
    //menu->exec(pixmapLabel->mapToGlobal(pos));
    menu->exec(glWidget->mapToGlobal(pos));
}

void MainWindow::reset()
{
    glWidget->reset();
    xSlider->setValue(0);
    refresh();
}

void MainWindow::finishedDrawing(){
    statusBar()->showMessage(tr("Finished."), 2000);
}

void MainWindow::open()
{
//    QString fileName = QFileDialog::getOpenFileName(
//                this,
//                "Select a CSV file to open",
//                QString("/Users/").append("Steven").append("/Desktop"),
//                " (*.csv)");
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

void MainWindow::resizeEvent(QResizeEvent *e)
{
  // call the base method
  QMainWindow::resizeEvent(e);
    glWidget->refreshTranslations();
  //this->setGL();
}

void MainWindow::setPixelmap()
{
    renderCount++;
    qDebug()<<"received, running setpixelmap";
    qDebug()<<renderCount;
    if(renderCount%2 == 0){
        renderCount = 0;
        pixmapLabel = new QLabel;
//        QImage image = glWidget->grabFrameBuffer();
//        setPixmap(QPixmap::fromImage(image));
        QImage image = glWidget->grabFrameBuffer();
        setPixmap(QPixmap::fromImage(image));

        pixmapLabel->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(pixmapLabel, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ctxMenu(const QPoint &)));
    }
    qDebug()<<"finished setting";
}

void MainWindow::setGL()
{
 //   qDebug() << model->dimensions.at(0)->title;
    glWidget = new GLObject;

    connect(glWidget, SIGNAL(valueChanged()),
            this, SLOT(setPixelmap()) );

    //glWidgetArea->hide();
    glWidgetArea->setWidget(glWidget);
    glWidget->model = this->model;
    //setPixmap(QPixmap());
    this->refresh();
}

void MainWindow::grabFrameBuffer()
{
    pixmapLabel = new QLabel;
    QImage image = glWidget->grabFrameBuffer();
    setPixmap(QPixmap::fromImage(image));
}

void MainWindow::clearPixmap()
{
 //   qDebug() << model->dimensions.at(0)->title;
    glWidget = new GLObject;

    //glWidgetArea->hide();
    glWidgetArea->setWidget(glWidget);
    glWidget->model = this->model;
    //setPixmap(QPixmap());
    this->refresh();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Bus Analyzer"),
            tr("Steven Pungdumri's Thesis:\n\nThis tool provides an interactive experience in visualizing workload data, allowing manipulation of the graph by reordering axes, concealing axes, highlighting particular ranges of data, scaling particular areas of interest, and viewing the machine suggested areas of interest."));
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    /*
    renderIntoPixmapAct = new QAction(tr("&Render into Pixmap..."), this);
    renderIntoPixmapAct->setShortcut(tr("Ctrl+R"));
    connect(renderIntoPixmapAct, SIGNAL(triggered()),
            this, SLOT(renderIntoPixmap()));

    grabFrameBufferAct = new QAction(tr("&Grab Frame Buffer"), this);
    grabFrameBufferAct->setShortcut(tr("Ctrl+G"));
    connect(grabFrameBufferAct, SIGNAL(triggered()),
            this, SLOT(grabFrameBuffer()));

    clearPixmapAct = new QAction(tr("&Clear Pixmap"), this);
    clearPixmapAct->setShortcut(tr("Ctrl+L"));
    connect(clearPixmapAct, SIGNAL(triggered()), this, SLOT(clearPixmap()));
*/

    resetAct = new QAction(tr("&Reset Transforms"), this);
    resetAct->setShortcut(tr("Ctrl+R"));
    connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    modeAct = new QAction(tr("&Mode"), this);
    connect(modeAct, SIGNAL(triggered()), this, SLOT(focusTest()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::refresh(){
//    glWidgetArea->viewport()->repaint();
//    glWidgetArea->viewport()->update();
//    QProgressBar p = QProgressBar(glWidget);

    statusBar()->showMessage(tr("Rendering..."), 2000);
    glWidgetArea->hide();
    glWidgetArea->show();    
}

void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    //textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
//    fileMenu->addAction(grabFrameBufferAct);
//    fileMenu->addAction(clearPixmapAct);
    fileMenu->addAction(resetAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(modeAct);
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

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

void MainWindow::setPixmap(const QPixmap &pixmap)
{
    pixmapLabel->setPixmap(pixmap);
    QSize size = pixmap.size();
    if (size - QSize(1, 0) == pixmapLabelArea->maximumViewportSize())
        size -= QSize(1, 0);
    pixmapLabel->resize(size);

    glWidgetArea->setWidget(pixmapLabel);
}

void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
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
}

QSize MainWindow::getSize()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Grabber"),
                                         tr("Enter pixmap size:"),
                                         QLineEdit::Normal,
                                         tr("%1 x %2").arg(glWidget->width())
                                                      .arg(glWidget->height()),
                                         &ok);
    if (!ok)
        return QSize();

    QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
    if (regExp.exactMatch(text)) {
        int width = regExp.cap(1).toInt();
        int height = regExp.cap(2).toInt();
        if (width > 0 && width < 2048 && height > 0 && height < 2048)
            return QSize(width, height);
    }

    return glWidget->size();
}
