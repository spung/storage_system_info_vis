#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QGLWidget>
#include <QFileDialog>
#include <QKeyEvent>


int main(int argc, char *argv[])
{
    //construct new Qt application
    QApplication a(argc, argv);

    // check for openGL support
    if(!QGLFormat::hasOpenGL()){
        qDebug("Your system doesn't seem to support OpenGL, please try again on a different system.");
        return EXIT_FAILURE;
    }


    //QString fileName = QFileDialog::getOpenFileName(obj, tr("Open File"),
      //                                               "",
        //                                             tr("Files (*.*)"));

    MainWindow win;
    win.show();


    return a.exec();
}
