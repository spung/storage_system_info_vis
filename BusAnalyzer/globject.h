/**
    Author: Steven Pungdumri
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/

#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QGLWidget>
#include <QLabel>
#include "analyzermodel.h"

// an openGL object to render the visualization
class GLObject : public QGLWidget{

    Q_OBJECT

    public:
        GLObject();
        ~GLObject();

        // model contains the dataset stored in various collections
        AnalyzerModel *model;

        // initialize the visualization
        void initializeGL();

        // resize the visualization
        void resizeGL(int width, int height);

        // render the visualization
        void paintGL();

        // key press callback
        void keyPressEvent(QKeyEvent *event);

        // change callback
        void changeEvent(QEvent *event);

        // set the main window of the visualization
        void setMain(QMainWindow *main);

        // convert from pixel to world coordinates
        double p2w_x(double xp);
        double p2w_y(double yp);

        // variables for scale and translation values
        float scaleVal, xTranslate, yTranslate;

        // last position for mouse movement (for translations)
        QPoint lastPos;

        // reset all transformations
        void reset();

    // signal callbacks in the mainwindow
    signals:
         void valueChanged();
         void doneRendering();

    // callbacks made from the mainwindow
    public slots:
        void refreshTranslations();

    // handle mouse move events
    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
};

#endif // GLOBJECT_H
