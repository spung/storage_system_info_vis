/**
    Author: Steven Pungdumri
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/

#include "globject.h"
#include <QKeyEvent>

// constructor
GLObject::GLObject(){}

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

// reset and reinitialize all transformation variables
void GLObject::reset(){
    refreshTranslations();
    scaleVal = 0.0;
}

// reset and reinitialize all translation variables
void GLObject::refreshTranslations(){
    xTranslate = width()/2.0;
    yTranslate = height()/2.0;
}

// capture mouse events
void GLObject::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    // for left button click and drags, calculate translation values
    if(Qt::LeftButton){
        if(scaleVal != 0){
            xTranslate += dx/(scaleVal*4.0);
            yTranslate += dy/(scaleVal*4.0);
        }
        else{
            xTranslate += dx;
            yTranslate += dy;
        }
    }

    lastPos = event->pos();

    emit valueChanged();
}

// capture mouse presses for calculating drag movements for translations
void GLObject::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
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

// draws a circle with specified x and y coordinates, radius, and number of segments desired
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

// openGL painting code
void GLObject::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen and depth buffer
    glLoadIdentity(); // reset current modelview matrix

    double smallVal = 0.000000000001;
    double labelZVal = -1.0*smallVal;
    double recordZVal = 0.0;
    double axesZVal = smallVal;

    if(model){
        double xVal1, yVal1, xVal2, yVal2, drawX1, drawX2, drawY1, drawY2, top, right = 0;
        float axisWidth = 0.005;
        top = p2w_y(this->height());
        right = p2w_x(this->width());
        Dimension *currDim, *nextDim = NULL;

        double horizontalInc = this->width()/(model->order.size()-1);

        glPushMatrix();
        glTranslatef(p2w_x(xTranslate), - p2w_y(yTranslate), -2.5f + 2.0*scaleVal); // Move 2.5 into the screen

        // draw axes
        drawY1 = -p2w_y(0.0 * this->height());
        drawY2 = -p2w_y(1.0 * this->height());

        int orderCount = model->order.size();
        float minPosition = -p2w_y(0.02 * this->height());
        float maxPosition = -p2w_y(0.98 * this->height());
        for(int currentDimPos = 0; currentDimPos < orderCount;currentDimPos++){
            drawX1 = p2w_x(horizontalInc * currentDimPos);
            glColor4f(0.0, 0.0, 0.0, 0.15); // set color
            glBegin(GL_QUADS);
                glVertex3f(drawX1 - axisWidth, drawY1, axesZVal);
                glVertex3f(drawX1 - axisWidth, drawY2, axesZVal);
                glVertex3f(drawX1 + axisWidth, drawY2, axesZVal);
                glVertex3f(drawX1 + axisWidth, drawY1, axesZVal);
            glEnd();

            glColor3f(0.0, 0.0, 0.0);
            drawX1 -= 0.05;
            this->renderText(drawX1, drawY1 + 0.002 + p2w_y(yTranslate) - 0.8*scaleVal, labelZVal, QString("%1").arg(model->dimensions.at(model->order.at(currentDimPos))->title, 7, ' '));

            glColor3f(1.0, 0.0, 0.0);

            Dimension *currentDim = model->dimensions.at(model->order.at(currentDimPos));
            int values = currentDim->getNameValuesSize();
            if(values != 0){
                Discrete *currentDiscrete = dynamic_cast<Discrete*>(currentDim);
                for(int i = 0; i<values; i++){

                    if(i == 0){
                        this->renderText(drawX1, minPosition, labelZVal, currentDiscrete->getNameValueAt(i));
                    }
                    else
                        this->renderText(drawX1, -p2w_y(i*1.0/(values-1) * this->height()), labelZVal, currentDiscrete->getNameValueAt(i));

                    // draw histogram
                    double histValue = model->getHistogramValue(currentDiscrete->id, i);

                    float histogramWidth =  fabs(p2w_x(horizontalInc) - p2w_x(horizontalInc*2)) * histValue;
                    glColor4f(1.0, 0.0, 0.0, 0.15);
                    circle(drawX1 + 0.05, -p2w_y(i*1.0/(values-1) * this->height()), histogramWidth/2, 36);

                    //  square/rectangle histograms
                    /*
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
                    this->renderText(drawX1 - axisWidth, minPosition, labelZVal, QString("%1").arg(tempMin, 0, 'g', 6));
                    this->renderText(drawX1 - axisWidth, maxPosition, labelZVal, QString("%1").arg(tempMax, 0, 'g', 6));
                }
                else{
                    this->renderText(drawX1, minPosition, labelZVal, QString("%1").arg(model->dimensions.at(model->order.at(currentDimPos))->currentMin, 0, 'g', 6));
                    this->renderText(drawX1, maxPosition, labelZVal, QString("%1").arg(model->dimensions.at(model->order.at(currentDimPos))->currentMax, 0, 'g', 6));
                }
            }
        }

        // draw records
        int counter = 0;

        foreach(Record *rec, model->records){            
            if(model->mode == MODE_OVERVIEW ||
                    (model->mode == MODE_FOCUS && rec->at(model->focus_dimension->id) <= model->focus_max && rec->at(model->focus_dimension->id) >= model->focus_min)){

                if(model->colorThreads){
                    glColor4f(rec->color.redF(), rec->color.greenF(), rec->color.blueF(), 0.55);
                }
                else if(rec->brushed){
                    glColor4f(1.0, 0.0, 0.0, 0.15);
                }
                else{
                    glColor4f(0.0, 0.0, 0.0, 0.15);
                }
                for(int currentDimPos = 0; currentDimPos < orderCount;currentDimPos++){
                    currDim = model->dimensions.at(model->order.at(currentDimPos));

                    if((currentDimPos + 1) < model->order.size()){
                        nextDim = model->dimensions.at(model->order.at(currentDimPos+1));

                        xVal1 = currentDimPos;
                        xVal2 = currentDimPos + 1;

                        yVal1 = currDim->getValue(model->mode, rec->at(model->order.at(currentDimPos)));
                        yVal2 = nextDim->getValue(model->mode, rec->at(model->order.at(currentDimPos + 1)));

                        drawY1 = -p2w_y(yVal1 * this->height());
                        drawY2 = -p2w_y(yVal2 * this->height());

                        drawX1 =  p2w_x(horizontalInc*currentDimPos);
                        drawX2 =  p2w_x(horizontalInc*(currentDimPos+1));

                        glBegin(GL_LINES);
                            glVertex3f(drawX1, drawY1, recordZVal);
                            glVertex3f(drawX2, drawY2, recordZVal);
                        glEnd();
                    }
                }
                counter++;
            }
        }
        glPopMatrix();

        emit doneRendering();
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
