TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    LogisticRegression.cpp \
    functions.cpp \
    HiddenLayer.cpp \
    RBM.cpp \
    CRBM.cpp \
    DBN.cpp \
    CDBN.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    LogisticRegression.h \
    functions.h \
    HiddenLayer.h \
    RBM.h \
    CRBM.h \
    DBN.h \
    CDBN.h

OTHER_FILES += \
    BoostCDBN.pro.user


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lquadmath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lquadmath
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lquadmath

INCLUDEPATH += $$PWD/../../../../../../../usr/lib/gcc/x86_64-unknown-linux-gnu/4.9.1/include
DEPENDPATH += $$PWD/../../../../../../../usr/lib/gcc/x86_64-unknown-linux-gnu/4.9.1/include
