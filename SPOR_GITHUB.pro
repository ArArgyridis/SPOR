TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wall

SOURCES += main.cpp \
    ontologyclass.cpp \
    ontologyparser.cpp \
    ontologycontainer.cpp \
    logicnode.cpp \
    complementnode.cpp \
    ornode.cpp \
    classrelatedfeaturenode.cpp \
    treenode.cpp \
    featurenode.cpp \
    fuzzyevaluator.cpp \
    fuzzyfunctions.cpp \
    helperfunctions.cpp \
    superclassnode.cpp \
    ontologydata.cpp \
    fuzzyobjectproperty.cpp \
    fuzzydatatype.cpp \
    andnode.cpp

include(deployment.pri)
qtcAddDeployment()

SUBDIRS += \
    FuzzyOWLReasonerRC1.pro \
    FuzzyOWLReasonerRC1.pro

DISTFILES += \
    SPOR_GITHUB.pro.user

HEADERS += \
    ontologyclass.h \
    ontologydata.h \
    treenode.h \
    logicnode.h \
    ontologyparser.h \
    fuzzyevaluator.h \
    helperfunctions.h \
    complementnode.h \
    superclassnode.h \
    ornode.h \
    classrelatedfeaturenode.h \
    featurenode.h \
    ontologycontainer.h \
    fuzzyobjectproperty.h \
    fuzzyfunctions.h \
    fuzzydatatype.h \
    andnode.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/lib/release/ -lxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/lib/debug/ -lxml2
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/lib/ -lxml2

INCLUDEPATH += $$PWD/../../../../../../../../usr/include/libxml2
DEPENDPATH += $$PWD/../../../../../../../../usr/include/libxml2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/lib/release/ -lpqxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/lib/debug/ -lpqxx
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/lib/ -lpqxx

INCLUDEPATH += $$PWD/../../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../../usr/include
