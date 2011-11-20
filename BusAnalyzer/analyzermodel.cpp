#include "analyzermodel.h"

AnalyzerModel::AnalyzerModel()
{
    mode = MODE_OVERVIEW;
}

bool AnalyzerModel::inFocus(Record*rec){
    double value = rec->at(focus_dimension->id);
    if(value >= focus_min && value <= focus_max)
        return true;
    else
        return false;
}

bool AnalyzerModel::isEnumDim(int dim){
    if(dim == QUEUEABLE || dim == CMD || dim == ALIGNMENT || dim == FIFO_POS || dim == STREAM_NUM || dim == CACHE_HIT){
        return true;
    }
    else{
        return false;
    }
}

typedef struct {
  int timeToLive;
  QColor color;
} Thread;

void AnalyzerModel::generateColorThreads(){
    QMap<int, Thread> liveThreads;
    long currentThread;
    long threadCount = 0;
    Thread insertThread;
    bool newThread = false;

    // get the largest queue value of this dataset
    int reach = dimensions.at(QUEUE_DEPTH)->max;

    // set seed to 1 so the colors are always the same for this dataset
    srand(1);

    int lbaDelta = 1000;

    // go through each record
    foreach(Record *rec, records){
        // identify threads

        // get LBA value
        currentThread = rec->at(LBA);

        // if the thread is present, set this rec to that color
        QMapIterator<int, Thread> iter(liveThreads);

        while(iter.hasNext() && !(currentThread >= iter.peekNext().key() - lbaDelta && currentThread <= iter.peekNext().key() + lbaDelta)){
            iter.next();
        }

        // this belongs to a preexisting thread in liveThreads
        if(currentThread >= iter.peekNext().key() - lbaDelta && currentThread <= iter.peekNext().key() + lbaDelta){
            newThread = false;
            rec->color = iter.peekNext().value().color;
            insertThread.color = rec->color;
            liveThreads.remove(iter.peekNext().key());
        }
        // this is a new thread not in liveThreads
        else{
            newThread = true;
            threadCount++;

            qDebug() << QString("thread: %1").arg(QString::number(threadCount));

            // set the color of this thread to the next random color
            QColor col = QColor::fromHsv(rand() % 360, 255, 255, 255);
            insertThread.color = col;
            rec->color = insertThread.color;
        }

//        if(liveThreads.contains(currentThread)){
//            insertThread = liveThreads.value(currentThread);
//            rec->color = insertThread.color;
//            liveThreads.remove(currentThread);
//        }
//        // else insert the new thread into liveThreads
//        else{
//            threadCount++;

//            // set the color of this thread to the next random color
//            QColor col = QColor::fromHsv(rand() % 360, 255, 255, 255);
//            insertThread.color = col;
//            rec->color = insertThread.color;
//        }

        // reset time to live for this thread
        insertThread.timeToLive = reach;

        // the next expected LBA for this thread should be this LBA + the cmd length
        liveThreads.insert(currentThread, insertThread);

        if(newThread){
            // decrement all other live threads
            iter.toFront();
            while(iter.hasNext()){
                if(iter.peekNext().key() != currentThread){
                    long currentIterKey = iter.peekNext().key();
                    Thread currentIterVal = iter.peekNext().value();
                    currentIterVal.timeToLive--;
                    if(currentIterVal.timeToLive == 0){
                        liveThreads.remove(currentIterKey);
                    }
                    else{
                        liveThreads.insert(currentIterKey, currentIterVal);
                    }
                }
                iter.next();
            }
        }
    }
    qDebug() << QString("total threads: %1").arg(QString::number(threadCount));
}

void AnalyzerModel::setFocus(Dimension*dim, double min, double max){
    this->focus_dimension = dim;
    this->focus_min = min;
    this->focus_max = max;
    this->mode = MODE_FOCUS;

    // initialize each dimension's current max/min
    for(int currentDimPos = 0; currentDimPos < order.size(); currentDimPos++){
        dimensions.at(currentDimPos)->currentMin =  DBL_MAX;
        dimensions.at(currentDimPos)->currentMax = 0;
    }

    foreach(Record *rec, records){
        // go through each record in the user selected focus
        if(inFocus(rec)){
            // calculate new currentMin/currentMax for each dimension
            for(int currentDimPos = 0; currentDimPos < order.size(); currentDimPos++){
                // ignore the dimensions that are enum type dimensions
                if(!isEnumDim(currentDimPos)){
                    double currVal = rec->at(currentDimPos);
                    if(currVal < dimensions.at(currentDimPos)->currentMin){
                        dimensions.at(currentDimPos)->currentMin = currVal;
                    }
                    if(currVal > dimensions.at(currentDimPos)->currentMax){
                        dimensions.at(currentDimPos)->currentMax = currVal;
                    }
                }
                else{
                    dimensions.at(currentDimPos)->currentMin = dimensions.at(currentDimPos)->min;
                    dimensions.at(currentDimPos)->currentMax = dimensions.at(currentDimPos)->max;
                }
            }
        }
    }
}

void AnalyzerModel::setBrushCriteria(int dimension, double min, double max){
    Dimension *tempDim = dimensions.at(dimension);
    this->brush_dimension = tempDim;
    brush_min = min;
    brush_max = max;
    foreach(Record *rec, records){
        if(rec->at(dimension) >= min && rec->at(dimension) <= max ){
            rec->brushed = true;
        }
        else
            rec->brushed = false;
    }
}

double AnalyzerModel::getBrushMin(){
    return brush_min;
}

double AnalyzerModel::getBrushMax(){
    return brush_max;
}

double AnalyzerModel::getFocusMax(){
    return focus_max;
}

double AnalyzerModel::getFocusMin(){
    return focus_min;
}

Dimension* AnalyzerModel::getBrushDimension(){
    return brush_dimension;
}

Dimension* AnalyzerModel::getFocusDimension(){
    return focus_dimension;
}

void AnalyzerModel::printRecords(){
    foreach(Record *label, records )  {
        qDebug() << label->print();
    }
}

void AnalyzerModel::initOrder(){
    order.remove(4);
    order.push_back(QUEUEABLE);
    order.remove(4);
    order.push_back(CMD);
    order.remove(4);
    order.push_back(INTER_CMD_TIME);
}

void AnalyzerModel::hideAt(int position){
    dimensions.at(order.at(position))->visible = false;
    hidden.insert(dimensions.at(order.at(position))->title, order.at(position));
    //qDebug()<< QString("hid the title = %1, at = %2").arg(dimensions.at(order.at(position))->title).arg(QString::number(order.at(position)));
    order.remove(position);
}

void AnalyzerModel::insertAt(int position, QString *text){
    int dimensionValue = hidden.value(*text);
    //qDebug()<< QString("Got here, position = %1, text = %2, dimensionValue: %3").arg(QString::number(position)).arg(*text).arg(QString::number(dimensionValue));
    hidden.remove(*text);
    order.insert(position, dimensionValue);
    dimensions.at(dimensionValue)->visible = true;
}

void AnalyzerModel::setLinear(int position){
    dimensions.at(order.at(position))->scaleEq = new Linear();
}

void AnalyzerModel::setLog(int position){
    dimensions.at(order.at(position))->scaleEq = new LogTen();
}

bool AnalyzerModel::loadFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        error = file.errorString();
        return false;
    }

    QTextStream in(&file);

    QStringList list;
    long count;

    // create dimensions
    list = in.readLine().split(",");
    count = list.size();

    // dimensions
    for(int i = 0; i<count;i++){
        Discrete *discretePointer = new Discrete(i, list.at(i), true, new Linear());
        Dimension *newDim = discretePointer;
        switch(i){
        case QUEUEABLE:
            discretePointer->insertNameValue(new QString("False"));
            discretePointer->insertNameValue(new QString("True"));
            discretePointer->initCount(2);
            break;
        case CMD:
            discretePointer->insertNameValue(new QString("-"));
            discretePointer->insertNameValue(new QString("Write"));
            discretePointer->insertNameValue(new QString("Read"));
            discretePointer->insertNameValue(new QString("F"));
            discretePointer->initCount(4);
            break;
        case ALIGNMENT:
            discretePointer->insertNameValue(new QString("None"));
            discretePointer->insertNameValue(new QString("Aligned End"));
            discretePointer->insertNameValue(new QString("Aligned End Stream"));
            discretePointer->insertNameValue(new QString("Aligned Start"));
            discretePointer->insertNameValue(new QString("Aligned Start Stream"));
            discretePointer->insertNameValue(new QString("Aligned Fully"));
            discretePointer->insertNameValue(new QString("Aligned Fully Stream"));
            discretePointer->insertNameValue(new QString("Unaligned Fully"));
            discretePointer->initCount(8);
            break;
        case FUA:
            discretePointer->insertNameValue(new QString("False"));
            discretePointer->insertNameValue(new QString("True"));
            discretePointer->initCount(2);
            break;
        case SEQUENTIAL:
            discretePointer->insertNameValue(new QString("None"));
            discretePointer->insertNameValue(new QString("Sequential"));
            discretePointer->insertNameValue(new QString("Sequential Stream"));
            discretePointer->initCount(3);
            break;
        case CACHE_HIT:
            discretePointer->insertNameValue(new QString("Miss"));
            discretePointer->insertNameValue(new QString("Hit"));
            discretePointer->initCount(2);
            break;
        default:
            if(i == INTER_CMD_TIME || i == CCT || i == QUEUE_CCT || i == FIFO_POS ){
                newDim = new Continuous(i, list.at(i), true, new LogTen());
            }
            else{
                newDim = new Continuous(i, list.at(i), true, new Linear());
            }
            break;
        }
        dimensions.append(newDim);

        dimensions.at(i)->min = DBL_MAX;
        dimensions.at(i)->max = 0;
        order.append(i);
    }

    list.clear();
    //QVector<Record> records;
    //records.reserve(5);

    // records
    Record *tempRecord;
    long current = 0;
    double currentValue = 0;
    while(!in.atEnd()){

        // comma separate each line
        list = in.readLine().split(",");

        // append a new record
        records.append(new Record());

        // point to the newly inserted record
        tempRecord = records[current];
        tempRecord->brushed = false;
        current++;

        for(int i = 0; i<count;i++){
            currentValue = list.at(i).toDouble();
            switch (i){
            case 0:
                tempRecord->startTime = list.at(i).toDouble();
                break;
            case 1:
                tempRecord->endTime = list.at(i).toDouble();
                break;
            case 2:
                tempRecord->startId = list.at(i).toLong();
                break;
            case 3:
                tempRecord->endId = list.at(i).toLong();
                break;
            case 4:
                tempRecord->queueable = (list.at(i).compare(QString('q')) == 0) ? 1 : 0;
                currentValue = tempRecord->queueable;
                dimensions.at(4)->incrementCount(currentValue);
                //qDebug() << QString("value: %1, compare to: %2, equals: %3, queueable: %4").arg(list.at(i)).arg(QString('q')).arg(QString::number(list.at(i).compare(QString('q')))).arg(QString::number(tempRecord->queueable));
                break;
            case 5:
                if(list.at(i).compare(QString('W')) == 0){
                    tempRecord->cmd = COMMAND_WRITE;
                }
                else if(list.at(i).compare(QString('R')) == 0){
                    tempRecord->cmd = COMMAND_READ;
                }
                else if(list.at(i).compare(QString('F')) == 0){
                    tempRecord->cmd = COMMAND_F;
                }
                else if(list.at(i).compare(QString('-')) == 0){
                    tempRecord->cmd = COMMAND_NONE;
                }
                else{
                    tempRecord->cmd = COMMAND_NONE;
                }
                currentValue = tempRecord->cmd;
                dimensions.at(5)->incrementCount(currentValue);
                break;
            case 6:
                tempRecord->interCmdTime = list.at(i).toDouble();
                break;
            case 7:
                tempRecord->lba = list.at(i).toLong();
                break;
            case 8:
                tempRecord->length = list.at(i).toLong();
                break;
             case 9:
                if(list.at(i).compare(ALIGNED_END_TEXT) == 0)
                    tempRecord->alignment = ALIGNED_END;
                else if(list.at(i).compare(ALIGNED_END_STREAM_TEXT) == 0
                        || list.at(i).compare(ALIGNED_END_STREAM_TEXT_ALT) == 0)
                    tempRecord->alignment = ALIGNED_END_STREAM;
                else if(list.at(i).compare(ALIGNED_START_TEXT) == 0)
                    tempRecord->alignment = ALIGNED_START;
                else if(list.at(i).compare(ALIGNED_START_STREAM_TEXT) == 0)
                    tempRecord->alignment = ALIGNED_START_STREAM;
                else if(list.at(i).compare(ALIGNED_FULLY_TEXT) == 0)
                    tempRecord->alignment = ALIGNED_FULLY;
                else if(list.at(i).compare(ALIGNED_FULLY_STREAM_TEXT) == 0)
                    tempRecord->alignment = ALIGNED_FULLY_STREAM;
                else if(list.at(i).compare(ALIGNED_FULLY_UNALIGNED_TEXT) == 0)
                    tempRecord->alignment = ALIGNED_FULLY_UNALIGNED;
                else{
                    tempRecord->alignment = ALIGNED_NONE;
                    if(list.at(i).length()) qDebug() << "***alignment ERROR READING IN!! ** " + list.at(i);
                }
                currentValue = tempRecord->alignment;
                dimensions.at(9)->incrementCount(currentValue);
                break;
            case 10:
                tempRecord->fua = (list.at(i).compare(QString('N'))) ? 1 : 0;
                currentValue = tempRecord->fua;
                dimensions.at(10)->incrementCount(currentValue);
                break;
            case 11:
                tempRecord->cct = list.at(i).toDouble();
                break;
            case 12:
                tempRecord->qcct = list.at(i).toDouble();
                break;
            case 13:
                tempRecord->qDepth = list.at(i).toLong();
                break;
            case 14:
                tempRecord->eqDepth = list.at(i).toLong();
                break;
            case 15:
                tempRecord->fifoPos = list.at(i).toLong();
                break;
            case 16:
                if(list.at(i).compare(SEQ_TEXT) == 0 || list.at(i).compare(SEQ_TEXT_ALT) == 0)
                    tempRecord->sequential = SEQ;
                else if(list.at(i).compare(SEQ_STREAM_TEXT) == 0
                        || list.at(i).compare(SEQ_STREAM_TEXT_ALT) == 0)
                    tempRecord->sequential = SEQ_STREAM;
                else{
                    tempRecord->sequential = SEQ_NONE;
                    //if(list.at(i).length()) qDebug() << "***sequential ERROR READING IN!! ** " + list.at(i);
                }
                currentValue = tempRecord->sequential;
                dimensions.at(16)->incrementCount(currentValue);
                break;
            case 17:
                tempRecord->streamNum = list.at(i).toLong();
                break;
            case 18:
                tempRecord->cacheHit = (list.at(i).length() > 1) ? 1 : 0;
                currentValue = tempRecord->cacheHit;
                dimensions.at(18)->incrementCount(currentValue);
                break;
            default:
                break;
            }

            if (currentValue < dimensions.at(i)->min)
                dimensions.at(i)->min = currentValue;
            //if(i == 4) qDebug() << QString("current: %1, max: %2").arg(QString::number(list.at(i).toInt())).arg(QString::number(dimensions.at(i)->max));
            if (currentValue > dimensions.at(i)->max)
                dimensions.at(i)->max = currentValue;
        }
        list.clear();
    }

    for(int i = 0; i<count;i++){
        dimensions.at(i)->difference = dimensions.at(i)->max - dimensions.at(i)->min;
//        if(dimensions.at(i)->max == dimensions.at(i)->min){
//            qDebug() << "trying to remove";
//            order.remove(i);
//            qDebug() << "successfully removed";
//            dimensions.at(i)->visible = false;
//        }
        dimensions.at(i)->currentMin = dimensions.at(i)->min;
        dimensions.at(i)->currentMax = dimensions.at(i)->max;
//        qDebug() << QString("dimension: %1, min: %2, max: %3").arg(QString::number(i)).arg(QString::number(dimensions.at(i)->min)).arg(QString::number(dimensions.at(i)->max));
    }
    count = order.size();
    for(int i = 0; i<count;i++){
//        qDebug() << QString("dimension: %1, min: %2, max: %3").arg(QString::number(i)).arg(QString::number(dimensions.at(i)->min)).arg(dimensions.at(i)->max, 0, 'g', 8);
    }

    count = records.size();
    qDebug() << "done";
    qDebug() << count;

    file.close();

    generateColorThreads();
    colorThreads = false;

    initOrder();

    return true;
}
