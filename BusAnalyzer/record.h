/**
    Author: Steven Pungdumri
    Thesis: An Interactive Visualization Model for Analyzing Data Storage System Workloads
    2/6/2012
**/

#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QColor>

// numerical values for Command types
#define COMMAND_WRITE 1
#define COMMAND_READ 2
#define COMMAND_F 3
#define COMMAND_NONE 0

// string values for Alignment types when parsing input data
#define ALIGNED_END_TEXT            "aligned end"
#define ALIGNED_END_STREAM_TEXT     "aligned end (stream)"
#define ALIGNED_END_STREAM_TEXT_ALT     "aligned end (stream)"
#define ALIGNED_START_TEXT          "aligned start"
#define ALIGNED_START_STREAM_TEXT   "aligned start (stream)"
#define ALIGNED_FULLY_TEXT          "fully aligned"
#define ALIGNED_FULLY_STREAM_TEXT   "fully aligned (stream)"
#define ALIGNED_FULLY_UNALIGNED_TEXT   "fully unaligned"

// numerical values for Alignment types
#define ALIGNED_END             1
#define ALIGNED_END_STREAM      2
#define ALIGNED_START           3
#define ALIGNED_START_STREAM    4
#define ALIGNED_FULLY           5
#define ALIGNED_FULLY_STREAM    6
#define ALIGNED_FULLY_UNALIGNED 7
#define ALIGNED_NONE            0

// string values for Sequential types when parsing input data
#define SEQ_TEXT_ALT    "Sequential"
#define SEQ_TEXT          "seq"
#define SEQ_STREAM_TEXT   "seq (stream)"
#define SEQ_STREAM_TEXT_ALT   "seq (stream)"

// numerical values for Sequential types
#define SEQ           1
#define SEQ_STREAM    2
#define SEQ_NONE      0

// record is a class used for each entry in the dataset
class Record
{
public:
    Record();

    // the value at each dimension
    double startTime, endTime, interCmdTime, cct, qcct;
    short queueable, cmd, alignment, fua, sequential, cacheHit;
    uint32_t lba, startId, endId, length, qDepth, eqDepth, fifoPos, streamNum;

    // whether this record is brushed
    bool brushed;

    // the thread color of this record
    QColor color;

    // the thread id this record belongs to
    int thread;

    // helper function to print the values of this record
    QString print();

    // returns the value for this record at a particular dimension
    double at(int index);
};

Q_DECLARE_TYPEINFO( Record, Q_MOVABLE_TYPE );

#endif // RECORD_H
