/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Oct 12 08:42:45 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../BusAnalyzer/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      20,   11,   11,   11, 0x0a,
      34,   11,   11,   11, 0x0a,
      44,   11,   11,   11, 0x08,
      62,   11,   11,   11, 0x08,
      76,   11,   11,   11, 0x08,
      84,   11,   11,   11, 0x08,
      91,   11,   11,   11, 0x08,
     105,  101,   11,   11, 0x08,
     121,  101,   11,   11, 0x08,
     144,  101,   11,   11, 0x08,
     164,   11,   11,   11, 0x08,
     176,   11,   11,   11, 0x08,
     185,   11,   11,   11, 0x08,
     197,   11,   11,   11, 0x08,
     205,   11,   11,   11, 0x08,
     213,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0setGL()\0setPixelmap()\0"
    "refresh()\0grabFrameBuffer()\0clearPixmap()\0"
    "about()\0open()\0hideDim()\0pos\0"
    "ctxMenu(QPoint)\0dimensionClick(QPoint)\0"
    "marginClick(QPoint)\0setLinear()\0"
    "setLog()\0insertDim()\0brush()\0reset()\0"
    "finishedDrawing()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setGL(); break;
        case 1: setPixelmap(); break;
        case 2: refresh(); break;
        case 3: grabFrameBuffer(); break;
        case 4: clearPixmap(); break;
        case 5: about(); break;
        case 6: open(); break;
        case 7: hideDim(); break;
        case 8: ctxMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 9: dimensionClick((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 10: marginClick((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 11: setLinear(); break;
        case 12: setLog(); break;
        case 13: insertDim(); break;
        case 14: brush(); break;
        case 15: reset(); break;
        case 16: finishedDrawing(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
