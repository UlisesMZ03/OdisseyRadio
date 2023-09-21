/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../OdisseyRadio/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "actualizarMemoriaEnUso",
    "",
    "actualizarBarraProgreso",
    "cambiarPosicionReproduccion",
    "value",
    "cargarCancion",
    "detenerReproduccion",
    "iniciarReproduccion",
    "cargarCancionSeleccionada",
    "formatTiempo",
    "std::string",
    "tiempoSegundos"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[26];
    char stringdata0[11];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[24];
    char stringdata4[28];
    char stringdata5[6];
    char stringdata6[14];
    char stringdata7[20];
    char stringdata8[20];
    char stringdata9[26];
    char stringdata10[13];
    char stringdata11[12];
    char stringdata12[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 22),  // "actualizarMemoriaEnUso"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 23),  // "actualizarBarraProgreso"
        QT_MOC_LITERAL(59, 27),  // "cambiarPosicionReproduccion"
        QT_MOC_LITERAL(87, 5),  // "value"
        QT_MOC_LITERAL(93, 13),  // "cargarCancion"
        QT_MOC_LITERAL(107, 19),  // "detenerReproduccion"
        QT_MOC_LITERAL(127, 19),  // "iniciarReproduccion"
        QT_MOC_LITERAL(147, 25),  // "cargarCancionSeleccionada"
        QT_MOC_LITERAL(173, 12),  // "formatTiempo"
        QT_MOC_LITERAL(186, 11),  // "std::string"
        QT_MOC_LITERAL(198, 14)   // "tiempoSegundos"
    },
    "MainWindow",
    "actualizarMemoriaEnUso",
    "",
    "actualizarBarraProgreso",
    "cambiarPosicionReproduccion",
    "value",
    "cargarCancion",
    "detenerReproduccion",
    "iniciarReproduccion",
    "cargarCancionSeleccionada",
    "formatTiempo",
    "std::string",
    "tiempoSegundos"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    0,   63,    2, 0x08,    2 /* Private */,
       4,    1,   64,    2, 0x08,    3 /* Private */,
       6,    0,   67,    2, 0x08,    5 /* Private */,
       7,    0,   68,    2, 0x08,    6 /* Private */,
       8,    0,   69,    2, 0x08,    7 /* Private */,
       9,    0,   70,    2, 0x08,    8 /* Private */,
      10,    1,   71,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 11, QMetaType::Double,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'actualizarMemoriaEnUso'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actualizarBarraProgreso'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cambiarPosicionReproduccion'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'cargarCancion'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'detenerReproduccion'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'iniciarReproduccion'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cargarCancionSeleccionada'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'formatTiempo'
        QtPrivate::TypeAndForceComplete<std::string, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->actualizarMemoriaEnUso(); break;
        case 1: _t->actualizarBarraProgreso(); break;
        case 2: _t->cambiarPosicionReproduccion((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->cargarCancion(); break;
        case 4: _t->detenerReproduccion(); break;
        case 5: _t->iniciarReproduccion(); break;
        case 6: _t->cargarCancionSeleccionada(); break;
        case 7: { std::string _r = _t->formatTiempo((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::string*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
