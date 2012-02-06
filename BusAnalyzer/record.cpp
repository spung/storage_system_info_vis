#include "record.h"

Record::Record(){}

// helper function to print all values of a record
QString Record::print(){
    QString returnVal = "";
    returnVal.append(QString::number(startTime, 'g', 10) + ",");
    returnVal.append(QString::number(endTime, 'g', 10) + ",");
    returnVal.append(QString::number(startId) + ",");
    returnVal.append(QString::number(endId) + ",");
    returnVal.append(QString::number(queueable) + ",");
    returnVal.append(QString::number(cmd) + ",");
    returnVal.append(QString::number(interCmdTime, 'g', 10) + ",");
    returnVal.append(QString::number(lba) + ",");
    returnVal.append(QString::number(length) + ",");
    returnVal.append(QString::number(alignment) + ",");
    returnVal.append(QString::number(fua) + ",");
    returnVal.append(QString::number(cct, 'g', 10) + ",");
    returnVal.append(QString::number(qcct, 'g', 10) + ",");
    returnVal.append(QString::number(qDepth) + ",");
    returnVal.append(QString::number(eqDepth) + ",");
    returnVal.append(QString::number(fifoPos) + ",");
    returnVal.append(QString::number(sequential) + ",");
    returnVal.append(QString::number(streamNum) + ",");
    returnVal.append(QString::number(cacheHit) + ",");
    return returnVal;
}

// returns the value of the record at a particular dimension
double Record::at(int index){
    switch(index){
    case 0:
        return startTime;
    case 1:
        return endTime;
    case 2:
        return startId;
    case 3:
        return endId;
    case 4:
        return queueable;
    case 5:
        return cmd;
    case 6:
        return interCmdTime;
    case 7:
        return lba;
    case 8:
        return length;
    case 9:
        return alignment;
    case 10:
        return fua;
    case 11:
        return cct;
    case 12:
        return qcct;
    case 13:
        return qDepth;
    case 14:
        return eqDepth;
    case 15:
        return fifoPos;
    case 16:
        return sequential;
    case 17:
        return streamNum;
    case 18:
        return cacheHit;
    default:
        return 0;
    }
}
