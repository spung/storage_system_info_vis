/****************************************************************************
** Meta object code from reading C++ file 'globject.h'
**
** Created: Wed Jun 15 15:23:49 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../BusAnalyzer/globject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'globject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GLObject[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      25,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   41,    9,    9, 0x0a,
      65,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GLObject[] = {
    "GLObject\0\0valueChanged()\0doneRendering()\0"
    "angle\0setXRotation(int)\0refreshTranslations()\0"
};

const QMetaObject GLObject::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLObject,
      qt_meta_data_GLObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GLObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GLObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GLObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLObject))
        return static_cast<void*>(const_cast< GLObject*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GLObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged(); break;
        case 1: doneRendering(); break;
        case 2: setXRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: refreshTranslations(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void GLObject::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void GLObject::doneRendering()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
