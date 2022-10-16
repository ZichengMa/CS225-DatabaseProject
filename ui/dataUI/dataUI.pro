QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../Acsctrl.cpp \
    ../../Calc.cpp \
    ../../Calc_p.cpp \
    ../../Comparison.cpp \
    ../../DataInterface.cpp \
    ../../DataStorage.cpp \
    ../../DateTime.cpp \
    ../../FBH.cpp \
    ../../Hospital.cpp \
    ../../Record.cpp \
    ../../RecordPriority.cpp \
    ../../Simulator.cpp \
    ../../Utility.cpp \
    main.cpp \
    dataui.cpp \
    monthlyreport.cpp \
    simcond_dialog.cpp \
    weeklyreportshow.cpp

HEADERS += \
    ../../Acsctrl.h \
    ../../BPlus.h \
    ../../BPlus_multi.h \
    ../../Simulator.h \
    ../../SupClass.h \
    dataui.h \
    monthlyreport.h \
    simcond_dialog.h \
    weeklyreportshow.h

FORMS += \
    dataui.ui \
    monthlyreport.ui \
    simcond_dialog.ui \
    weeklyreportshow.ui

TRANSLATIONS += \
    dataUI_en_AS.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
