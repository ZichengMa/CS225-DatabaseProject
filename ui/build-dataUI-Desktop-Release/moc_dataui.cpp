/****************************************************************************
** Meta object code from reading C++ file 'dataui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dataUI/dataui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_dataUI_t {
    QByteArrayData data[15];
    char stringdata0[303];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dataUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dataUI_t qt_meta_stringdata_dataUI = {
    {
QT_MOC_LITERAL(0, 0, 6), // "dataUI"
QT_MOC_LITERAL(1, 7, 20), // "on_IncButton_clicked"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 24), // "on_SimruntButton_clicked"
QT_MOC_LITERAL(4, 54, 22), // "pon_run_symbol_timeout"
QT_MOC_LITERAL(5, 77, 22), // "pon_sim_driven_timeout"
QT_MOC_LITERAL(6, 100, 30), // "on_Conditionset_button_clicked"
QT_MOC_LITERAL(7, 131, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(8, 153, 27), // "on_QueryInput_returnPressed"
QT_MOC_LITERAL(9, 181, 19), // "on_Clear_pb_clicked"
QT_MOC_LITERAL(10, 201, 18), // "on_LoadNew_clicked"
QT_MOC_LITERAL(11, 220, 20), // "on_UpdateRec_clicked"
QT_MOC_LITERAL(12, 241, 18), // "on_Step_pb_clicked"
QT_MOC_LITERAL(13, 260, 20), // "on_WeekRP_pb_clicked"
QT_MOC_LITERAL(14, 281, 21) // "on_Monthrp_pb_clicked"

    },
    "dataUI\0on_IncButton_clicked\0\0"
    "on_SimruntButton_clicked\0"
    "pon_run_symbol_timeout\0pon_sim_driven_timeout\0"
    "on_Conditionset_button_clicked\0"
    "on_pushButton_clicked\0on_QueryInput_returnPressed\0"
    "on_Clear_pb_clicked\0on_LoadNew_clicked\0"
    "on_UpdateRec_clicked\0on_Step_pb_clicked\0"
    "on_WeekRP_pb_clicked\0on_Monthrp_pb_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dataUI[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    0,   90,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void dataUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<dataUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_IncButton_clicked(); break;
        case 1: _t->on_SimruntButton_clicked(); break;
        case 2: _t->pon_run_symbol_timeout(); break;
        case 3: _t->pon_sim_driven_timeout(); break;
        case 4: _t->on_Conditionset_button_clicked(); break;
        case 5: _t->on_pushButton_clicked(); break;
        case 6: _t->on_QueryInput_returnPressed(); break;
        case 7: _t->on_Clear_pb_clicked(); break;
        case 8: _t->on_LoadNew_clicked(); break;
        case 9: _t->on_UpdateRec_clicked(); break;
        case 10: _t->on_Step_pb_clicked(); break;
        case 11: _t->on_WeekRP_pb_clicked(); break;
        case 12: _t->on_Monthrp_pb_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject dataUI::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_dataUI.data,
    qt_meta_data_dataUI,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *dataUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dataUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_dataUI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int dataUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
