/****************************************************************************
** Meta object code from reading C++ file 'range.h'
**
** Created: Wed Jun 8 17:26:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../BusAnalyzer/range.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'range.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Range[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_Range[] = {
    "Range\0"
};

const QMetaObject Range::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Range,
      qt_meta_data_Range, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Range::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Range::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Range::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Range))
        return static_cast<void*>(const_cast< Range*>(this));
    return QDialog::qt_metacast(_clname);
}

int Range::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
