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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "globject.h"
#include "analyzermodel.h"
#include <typeinfo>
#include "range.h"
#include <QProgressBar>

#define ACTION_BRUSH 0
#define ACTION_FOCUS 1

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QSlider;
QT_END_NAMESPACE
class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    int sectionClicked;
    QPoint pointClicked;
    QProgressBar progress;

public slots:
    void setGL();
    void setPixelmap();
    void refresh();

private slots:
    //void renderIntoPixmap();
    void grabFrameBuffer();
    void clearPixmap();
    void about();
    void open();
    void hideDim();
    void ctxMenu(const QPoint &pos);
    void dimensionClick(const QPoint &pos);
    void marginClick(const QPoint &pos);
    void setLinear();
    void setLog();
    void insertDim();
    void focus();
    void brush();
    void getRange(short action);
    void reset();
    void finishedDrawing();
    void setOverviewMode();
    void setFocusMode();
    void setThreadMode();

private:
    void createActions();
    void loadFile(const QString &fileName);
    void createMenus();
    QSlider *createSlider(const char *changedSignal, const char *setterSlot);
    void setPixmap(const QPixmap &pixmap);
    QSize getSize();
    void setCurrentFile(const QString &fileName);
    void removeDim(int position);
    //void insertDim(int position, int dimension);

    QString curFile;

    QWidget *centralWidget;
    QScrollArea *glWidgetArea;
    QScrollArea *pixmapLabelArea;
    GLObject *glWidget;
    QLabel *pixmapLabel;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    QMenu *fileMenu;
    QMenu *modeMenu;
    QMenu *helpMenu;
    QAction *grabFrameBufferAct;
    //QAction *renderIntoPixmapAct;
    QAction *clearPixmapAct;
    QAction *exitAct;
    QAction *overviewModeAct;
    QAction *focusModeAct;
    QAction *threadModeAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *openAct;
    QAction *resetAct;

    AnalyzerModel *model;
    int renderCount;


protected:
    void resizeEvent(QResizeEvent *e);
};

#endif
