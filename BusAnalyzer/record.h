#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QColor>

// definitions specific to the command column of input data
#define COMMAND_WRITE 1
#define COMMAND_READ 2
#define COMMAND_F 3
#define COMMAND_NONE 0

// definitions specific to the alignment column of input data
#define ALIGNED_END_TEXT            "aligned end"
#define ALIGNED_END_STREAM_TEXT     "aligned end (stream)"
#define ALIGNED_END_STREAM_TEXT_ALT     "aligned end (stream)"
#define ALIGNED_START_TEXT          "aligned start"
#define ALIGNED_START_STREAM_TEXT   "aligned start (stream)"
#define ALIGNED_FULLY_TEXT          "fully aligned"
#define ALIGNED_FULLY_STREAM_TEXT   "fully aligned (stream)"
#define ALIGNED_FULLY_UNALIGNED_TEXT   "fully unaligned"
#define ALIGNED_END             1
#define ALIGNED_END_STREAM      2
#define ALIGNED_START           3
#define ALIGNED_START_STREAM    4
#define ALIGNED_FULLY           5
#define ALIGNED_FULLY_STREAM    6
#define ALIGNED_FULLY_UNALIGNED 7
#define ALIGNED_NONE            0

// definitions specific to the sequence column of input data
#define SEQ_TEXT          "seq"
#define SEQ_STREAM_TEXT   "seq (stream)"
#define SEQ_STREAM_TEXT_ALT   "seq (stream)"
#define SEQ           1
#define SEQ_STREAM    2
#define SEQ_NONE      0

class Record
{
public:
    Record();
    double startTime, endTime, interCmdTime, cct, qcct;
    short queueable, cmd, alignment, fua, sequential, cacheHit;
    uint32_t lba, startId, endId, length, qDepth, eqDepth, fifoPos, streamNum;

    bool brushed;
    QColor color;
    int thread;

    QString print();
    double at(int index);
};

Q_DECLARE_TYPEINFO( Record, Q_MOVABLE_TYPE );

#endif // RECORD_H
