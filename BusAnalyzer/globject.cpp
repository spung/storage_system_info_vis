#include "globject.h"
#include <QKeyEvent>

// constructor
GLObject::GLObject()
{
    setWindowTitle("Bus Analyzer");
}

// empty destructor
GLObject::~GLObject(){}

//initialize OpenGL
void GLObject::initializeGL(){
    //glShadeModel(GL_SMOOTH); // enable smooth shading
    qglClearColor(Qt::white); // set the clear color to be a black background

    glClearDepth(1.0f); //depth buffer setup
    glEnable(GL_DEPTH_TEST); // enable depth testing
    glDepthFunc(GL_LEQUAL); // set the type of depth test

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // really nice perspective calculations

    // enable blending, disable depth test
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    model = NULL;
    reset();
}

void GLObject::reset(){
    refreshTranslations();
    scaleVal = 0.0;
}

void GLObject::refreshTranslations(){
    xTranslate = width()/2.0;
    yTranslate = height()/2.0;
}

void GLObject::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();


    if(Qt::LeftButton){
        if(scaleVal != 0){
            xTranslate += dx/(scaleVal*4.0);
            yTranslate += dy/(scaleVal*4.0);
        }
        else{
            xTranslate += dx;
            yTranslate += dy;
        }
        //qDebug()<<QString("moved: width: %1, height: %2").arg(width()).arg(height());
    }

    lastPos = event->pos();

    emit valueChanged();
}

void GLObject::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<QString("mouse clicked at: x: %1, y: %2").arg(event->x()).arg(event->y());
    lastPos = event->pos();
}

void GLObject::setXRotation(int value){
    scaleVal = value/10.0;
}

// called when the window is resized
void GLObject::resizeGL(int width, int height){
    // prevent divide by zero in the gluPerspective call
    if(height == 0)
        height = 1;

    glViewport(0, 0, width, height); //reset current viewport

    glMatrixMode(GL_PROJECTION); //select projection matrix
    glLoadIdentity(); //reset projection matrix

    gluPerspective(45.0f, static_cast<GLfloat>(width)/height, 0.1f, 100.0f); // calcuate aspect ratio

    glMatrixMode(GL_MODELVIEW); //select modelview matrix
    glLoadIdentity(); //reset modelview matrix
}

void circle(float x, float y, float r, int segments)
{
    glBegin( GL_TRIANGLE_FAN );
        glVertex2f(x, y);
        for( int n = 0; n <= segments; ++n ) {
            float const t = 2*M_PI*(float)n/(float)segments;
            glVertex2f(x + sin(t)*r, y + cos(t)*r);
        }
    glEnd();
}

// openGL painting code goes here
void GLObject::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen and depth buffer
    glLoadIdentity(); // reset current modelview matrix

//    glBegin(GL_LINES);
//        glVertex3f(-1.80f,  -1.0f, 0.0f); // Top Left
//        glVertex3f( 1.80f,  -1.0f, 0.0f); // Top Right
//    glEnd();

    if(model){
        double xVal1, yVal1, xVal2, yVal2, drawX1, drawX2, drawY1, drawY2, top, right = 0;
        float axisWidth = 0.005;
        top = p2w_y(this->height());
        right = p2w_x(this->width());
        Dimension *currDim, *nextDim = NULL;

        double horizontalInc = this->width()/(model->order.size()-1);

        glPushMatrix();
//        qDebug()<<QString("xTrans: %1, yTrans: %2").arg(xTranslate).arg(yTranslate);
        glTranslatef(0.0f + p2w_x(xTranslate), 0.0f - p2w_y(yTranslate), -2.5f + 2.0*scaleVal); // Move 2.5 into the screen

        // draw axes
        drawY1 = -p2w_y(0.0 * this->height());
        drawY2 = -p2w_y(1.0 * this->height());

        int orderCount = model->order.size();
        int windowIncr = this->width()/orderCount*1.01;
        float minPosition = -p2w_y(0.02 * this->height());
        float maxPosition = -p2w_y(0.98 * this->height());
        float worldHeight = -p2w_y(this->height());
        for(int currentDimPos = 0; currentDimPos < orderCount;currentDimPos++){
            drawX1 = p2w_x(horizontalInc * currentDimPos);
            //qDebug()<< QString("axis: %1 drawX1: %2").arg(currentDimPos).arg(drawX1);
            //qDebug()<< QString("current: %1  horizontal incr: %2  position: %3").arg(QString::number(model->order.at(currentDimPos))).arg(QString::number(p2w_x(horizontalInc))).arg(QString::number(drawX1));
            glColor4f(0.0, 0.0, 0.0, 0.15); // set color
            glBegin(GL_QUADS);
                glVertex3f(drawX1 - axisWidth, drawY1, 0.01);
                glVertex3f(drawX1 - axisWidth, drawY2, 0.01);
                glVertex3f(drawX1 + axisWidth, drawY2, 0.01);
                glVertex3f(drawX1 + axisWidth, drawY1, 0.01);
            glEnd();

            glColor3f(0.0, 0.0, 0.0);
            drawX1 -= 0.05;
            this->renderText(drawX1, drawY1 + 0.01, 0.01, QString("%1").arg(model->dimensions.at(model->order.at(currentDimPos))->title, 7, ' '));

            glColor3f(1.0, 0.0, 0.0);

            Dimension *currentDim = model->dimensions.at(model->order.at(currentDimPos));
            int values = currentDim->getNameValuesSize();
            if(values != 0){
                Discrete *currentDiscrete = dynamic_cast<Discrete*>(currentDim);
                for(int i = 0; i<values; i++){
                    if(i == 0){
                        this->renderText(drawX1, minPosition, 0.01, currentDiscrete->getNameValueAt(i));
                    }
                    else
                        this->renderText(drawX1, -p2w_y(i*1.0/(values-1) * this->height()), 0.01, currentDiscrete->getNameValueAt(i));

                    qDebug()<<QString("Dimension: %1 value: %2 count: %3").arg(currentDimPos).arg(i).arg(currentDiscrete->getCount(i));
                    float histogramWidth =  fabs(p2w_x(horizontalInc) - p2w_x(horizontalInc*2)) * currentDiscrete->getCount(i)/model->records.size();
                    //qDebug() << QString("horizInc: %1 count: %2 total_recs: %3 count/total_rec: %4 horizInc*count/total_rec: %5").arg(drawX1).arg(currentDiscrete->getCount(i)).arg(model->records.size()).arg(1.0*currentDiscrete->getCount(i)/model->records.size()).arg(histogramWidth);
                    glColor4f(1.0, 0.0, 0.0, 0.15);
                    circle(drawX1 + 0.05, -p2w_y(i*1.0/(values-1) * this->height()), histogramWidth/2, 36);
                    /*
                      squares
                    glBegin(GL_QUADS);
                        glVertex3f(drawX1 + 0.05- histogramWidth/2, -p2w_y(i*1.0/(values-1) * this->height()) + histogramHeight, 0.01);
                        glVertex3f(drawX1 + 0.05- histogramWidth/2, -p2w_y(i*1.0/(values-1) * this->height()) - histogramHeight, 0.01);
                        glVertex3f(drawX1 + 0.05+ histogramWidth/2, -p2w_y(i*1.0/(values-1) * this->height()) - histogramHeight, 0.01);
                        glVertex3f(drawX1 + 0.05+ histogramWidth/2, -p2w_y(i*1.0/(values-1) * this->height()) + histogramHeight, 0.01);
                    glEnd();*/
                    glColor4f(1.0, 0.0, 0.0, 1.0);
                }
            }
            else{
                double tempMin, tempMax;
                if(model->mode == MODE_OVERVIEW){
                    tempMin = model->dimensions.at(model->order.at(currentDimPos))->min;
                    tempMax = model->dimensions.at(model->order.at(currentDimPos))->max;
                }
                else if(model->mode == MODE_FOCUS){
                    tempMin = model->dimensions.at(model->order.at(currentDimPos))->currentMin;
                    tempMax = model->dimensions.at(model->order.at(currentDimPos))->currentMax;
                }

                if(currentDimPos == 0){
                    this->renderText(drawX1 - axisWidth, minPosition, 0.01, QString("%1").arg(tempMin, 0, 'g', 6));
                    this->renderText(drawX1 - axisWidth, maxPosition, 0.01, QString("%1").arg(tempMax, 0, 'g', 6));
                }
                else{
                    this->renderText(drawX1, minPosition, 0.01, QString("%1").arg(model->dimensions.at(model->order.at(currentDimPos))->currentMin, 0, 'g', 6));
                    this->renderText(drawX1, maxPosition, 0.01, QString("%1").arg(model->dimensions.at(model->order.at(currentDimPos))->currentMax, 0, 'g', 6));
                }
            }
/*
            switch(model->order.at(currentDimPos)){
            case 5:
                this->renderText(drawX1, minPosition, 0.01, "None");
                this->renderText(drawX1, -p2w_y(1.0/3.0 * this->height()), 0.01, "Write");
                this->renderText(drawX1, -p2w_y(2.0/3.0 * this->height()), 0.01, "Read");
                this->renderText(drawX1, maxPosition, 0.01, "F");
            break;
            case 9:
                this->renderText(drawX1, minPosition, 0.01, "None");
                this->renderText(drawX1, -p2w_y(1/7.0 * this->height()), 0.01, "End");
                this->renderText(drawX1, -p2w_y(2/7.0 * this->height()), 0.01, "End Stream");
                this->renderText(drawX1, -p2w_y(3/7.0 * this->height()), 0.01, "Start");
                this->renderText(drawX1, -p2w_y(4/7.0 * this->height()), 0.01, "Start Stream");
                this->renderText(drawX1, -p2w_y(5/7.0 * this->height()), 0.01, "Fully");
                this->renderText(drawX1, -p2w_y(6/7.0 * this->height()), 0.01, "Fully Stream");
                this->renderText(drawX1, maxPosition, 0.01, "Fully Unaligned");
            break;
            case 16:
                this->renderText(drawX1, minPosition, 0.01, "None");
                this->renderText(drawX1, -p2w_y(1/2.0 * this->height()), 0.01, "Seq");
                this->renderText(drawX1, maxPosition, 0.01, "Seq Stream");
            break;
            case 18:
                this->renderText(drawX1, minPosition, 0.01, "Miss");
                this->renderText(drawX1, maxPosition, 0.01, "Hit");
            break;
            default:
                double tempMin, tempMax;
                if(model->mode == MODE_OVERVIEW){
                    tempMin = model->dimensions.at(model->order.at(currentDimPos))->min;
                    tempMax = model->dimensions.at(model->order.at(currentDimPos))->max;
                }
                else if(model->mode == MODE_FOCUS){
                    tempMin = model->dimensions.at(model->order.at(currentDimPos))->currentMin;
                    tempMax = model->dimensions.at(model->order.at(currentDimPos))->currentMax;
                }

                if(currentDimPos == 0){
                    this->renderText(drawX1 - axisWidth, minPosition, 0.01, QString("%1").arg(tempMin, 0, 'g', 6));
                    this->renderText(drawX1 - axisWidth, maxPosition, 0.01, QString("%1").arg(tempMax, 0, 'g', 6));
                }
                else{
                    this->renderText(drawX1, minPosition, 0.01, QString("%1").arg(model->dimensions.at(model->order.at(currentDimPos))->currentMin, 0, 'g', 6));
                    this->renderText(drawX1, maxPosition, 0.01, QString("%1").arg(model->dimensions.at(model->order.at(currentDimPos))->currentMax, 0, 'g', 6));
                }

            break;
            }*/
        }

        // draw records
        int counter = 0;

        foreach(Record *rec, model->records){            
            if(model->mode == MODE_OVERVIEW ||
                    (model->mode == MODE_FOCUS && rec->at(model->focus_dimension->id) <= model->focus_max && rec->at(model->focus_dimension->id) >= model->focus_min)){

                if(model->colorThreads){
                    //qDebug() << QString("hue: %1 saturation: %2 brightness: %3").arg(QString::number(rec->color.hslHue())).arg(QString::number(rec->color.saturation())).arg(QString::number(rec->color.value()));
                    glColor4f(rec->color.redF(), rec->color.greenF(), rec->color.blueF(), 0.55);
                }
                else if(rec->brushed){
                    glColor4f(1.0, 0.0, 0.0, 0.15);
                    //glTranslatef(0.0f, 0.0f, 0.001f);
                }
                else{
                    glColor4f(0.0, 0.0, 0.0, 0.15);
                }
                for(int currentDimPos = 0; currentDimPos < orderCount;currentDimPos++){
                    currDim = model->dimensions.at(model->order.at(currentDimPos));
    //                if(!currDim->visible) currDim = model->dimensions.at(currentDimPos + 1);
                    if((currentDimPos + 1) < model->order.size()){
                        nextDim = model->dimensions.at(model->order.at(currentDimPos+1));
    //                    if(!nextDim->visible) nextDim = model->dimensions.at(currentDimPos + 2);

                        xVal1 = currentDimPos;
                        xVal2 = currentDimPos + 1;

                        yVal1 = currDim->getValue(model->mode, rec->at(model->order.at(currentDimPos)));
                        yVal2 = nextDim->getValue(model->mode, rec->at(model->order.at(currentDimPos + 1)));

                        drawY1 = -p2w_y(yVal1 * this->height());
                        drawY2 = -p2w_y(yVal2 * this->height());

                        drawX1 =  p2w_x(horizontalInc*currentDimPos);
                        drawX2 =  p2w_x(horizontalInc*(currentDimPos+1));

                        glBegin(GL_LINES);
                            //glColor3f(1.0f, 0.0f, 0.0f); // Red color
                            //qDebug() << QString("drawing: %1, %2 to %3, %4").arg(drawX1).arg(drawY1).arg(drawX2).arg(drawY2);
                            glVertex3f(drawX1, drawY1, 0.01);
                            glVertex3f(drawX2, drawY2, 0.01);
                        glEnd();

                        if(currentDimPos == 5){
    //                        if(drawY1 > this->height())
    //                            qDebug() << QString("over: record: %1, raw value:%2  height: %3  drawing: %4").arg(counter).arg(yVal2).arg(p2w_y(this->height())).arg(drawY2);
    //                        if(drawY2 > this->height())
    //                            qDebug() << QString("over: record: %1, current dimension:%2  height: %3  drawing: %4").arg(counter).arg(currentDimPos).arg(this->height()).arg(drawY2);
                        }
    //                    if(drawY1 > this->height())
    //                        qDebug() << QString("over: record: %1, current dimension:%2  height: %3  drawing: %4").arg(counter).arg(currentDimPos).arg(this->height()).arg(drawY1);
    //                    if(drawY2 > this->height())
    //                        qDebug() << QString("over: record: %1, current dimension:%2  height: %3  drawing: %4").arg(counter).arg(currentDimPos).arg(this->height()).arg(drawY2);
                    }
                }
                counter++;
                //if(rec->brushed)
                    //glTranslatef(0.0f, 0.0f, -0.001f);
            }
        }
        glPopMatrix();
        //qDebug()<<"done draw loop";

        emit doneRendering();
        //qDebug()<<"emitted value changed";
    }
}

// converts pixel to world coodinates
double GLObject::p2w_x(double xp)
{
    double GW = this->width();
    double GH = this->height();
    return (float)2*GW/GH*1/(float)(GW-1) * xp  + -(float)GW/GH;
}

// converts pixel to world coodinates
double GLObject::p2w_y(double yp)
{
    double GW = this->width();
    double GH = this->height();
    return (float)2/(float)(GH-1) * yp - 1;
}

// key handler
void GLObject::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_0:
        model->order.remove(0);
        model->dimensions.at(0)->visible = false;
        this->paintGL();
        qDebug() << "pressed 0";
        break;
    case Qt::Key_Escape:
        close(); //quit on escape
        break;
    case Qt::Key_F11:
        setWindowState(windowState() ^ Qt::WindowFullScreen); // toggle fullscreen on F11 key press
        break;
    default:
        QGLWidget::keyPressEvent(event); // let base class handle other keys
    }
}

void GLObject::changeEvent(QEvent *event){
    switch(event->type()){
    case QEvent::WindowStateChange:
        //hide cursor if the window is fullscreen, otherwise show it
        if(windowState() == Qt::WindowFullScreen)
            setCursor(Qt::BlankCursor);
        else
            unsetCursor();
        break;
    default:
        break;
    }
}
