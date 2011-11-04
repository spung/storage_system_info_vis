#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QGLWidget>
#include <QLabel>
#include "analyzermodel.h"

class GLObject : public QGLWidget{

    Q_OBJECT

    public:
        GLObject();
        ~GLObject();
        AnalyzerModel *model;
        void initializeGL();
        void resizeGL(int width, int height);
        void paintGL();
        void keyPressEvent(QKeyEvent *event);
        void changeEvent(QEvent *event);
        void setMain(QMainWindow *main);
        double p2w_x(double xp);
        double p2w_y(double yp);
        float scaleVal, xTranslate, yTranslate;
        QPoint lastPos;
        void reset();

    signals:
         void valueChanged();
         void doneRendering();

    public slots:
        void setXRotation(int angle);
        void refreshTranslations();

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
};

#endif // GLOBJECT_H
