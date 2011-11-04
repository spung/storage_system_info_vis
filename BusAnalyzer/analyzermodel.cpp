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

void AnalyzerModel::setFocus(Dimension*dim, double min, double max){
    this->focus_dimension = dim;
    this->focus_min = min;
    this->focus_max = max;

    this->mode = MODE_FOCUS_THREADS;

    double currVal = 0;

    for(int currentDimPos = 0; currentDimPos < order.size(); currentDimPos++){
        dimensions.at(order.at(currentDimPos))->currentMin = 9999999.9;
        dimensions.at(order.at(currentDimPos))->currentMax = 0;
    }

    foreach(Record *rec, records){
        if(inFocus(rec)){
                //(model->mode > 0 && rec->at(model->order.at(model->focus_dimension->id)) < model->focus_max && rec->at(model->order.at(model->focus_dimension->id)) > model->focus_min)){
            for(int currentDimPos = 0; currentDimPos < order.size(); currentDimPos++){
                currVal = rec->at(order.at(currentDimPos));
                if(currVal < dimensions.at(order.at(currentDimPos))->currentMin){
                    dimensions.at(order.at(currentDimPos))->currentMin = currVal;
                    qDebug() << QString("min: %1").arg(QString::number(dimensions.at(order.at(currentDimPos))->currentMin));

                }
                if(currVal > dimensions.at(order.at(currentDimPos))->currentMax){
                    dimensions.at(order.at(currentDimPos))->currentMax = currVal;
                    qDebug() << QString("max: %1").arg(QString::number(dimensions.at(order.at(currentDimPos))->currentMax));
                }
            }
        }
    }

}

void AnalyzerModel::setBrushCriteria(int dimension, double min, double max){
    foreach(Record *rec, records){
        if(rec->at(dimension) >= min && rec->at(dimension) <= max ){
            rec->brushed = true;
        }
        else
            rec->brushed = false;
    }
}

void AnalyzerModel::printRecords(){
    foreach(Record *label, records )  {
        qDebug() << label->print();
    }
    qDebug() << "done";
}

void AnalyzerModel::hideAt(int position){
    dimensions.at(order.at(position))->visible = false;
    hidden.insert(dimensions.at(order.at(position))->title, order.at(position));
    qDebug()<< QString("hid the title = %1, at = %2").arg(dimensions.at(order.at(position))->title).arg(QString::number(order.at(position)));
    order.remove(position);
}

void AnalyzerModel::insertAt(int position, QString *text){
    int dimensionValue = hidden.value(*text);
    qDebug()<< QString("Got here, position = %1, text = %2, dimensionValue: %3").arg(QString::number(position)).arg(*text).arg(QString::number(dimensionValue));
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
        if(i == 6 || i == 11 || i == 12 || i == 15){
            dimensions.append(new Dimension(i,list.at(i), true, new LogTen()));
        }
        else{
            dimensions.append(new Dimension(i,list.at(i), true, new Linear()));
        }
        dimensions.at(i)->min = 9999999.9;
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
                    if(list.at(i).length()) qDebug() << "***ERROR READING IN!! ** " + list.at(i);
                }
                currentValue = tempRecord->alignment;
                break;
            case 10:
                tempRecord->fua = (list.at(i).compare(QString('N'))) ? 0 : 1;
                currentValue = tempRecord->fua;
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
                if(list.at(i).compare(SEQ_TEXT) == 0)
                    tempRecord->sequential = SEQ;
                else if(list.at(i).compare(SEQ_STREAM_TEXT) == 0
                        || list.at(i).compare(SEQ_STREAM_TEXT_ALT) == 0)
                    tempRecord->sequential = SEQ_STREAM;
                else{
                    tempRecord->sequential = SEQ_NONE;
                    if(list.at(i).length()) qDebug() << "***ERROR READING IN!! ** " + list.at(i);
                }
                currentValue = tempRecord->sequential;
                break;
            case 17:
                tempRecord->streamNum = list.at(i).toLong();
                break;
            case 18:
                tempRecord->cacheHit = (list.at(i).length() > 1) ? 1 : 0;
                currentValue = tempRecord->cacheHit;
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
        if(dimensions.at(i)->max == dimensions.at(i)->min){
            order.remove(i);
            dimensions.at(i)->visible = false;
        }
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
    return true;
}
