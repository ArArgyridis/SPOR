TEMPLATE = app
CONFIG += console
CONFIG -= qt
QMAKE_CXXFLAGS += -fopenmp -DNDEBUG -DBOOST_UBLAS_NDEBUG
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wall

SOURCES += main.cpp \
    fuzzyfunctions.cpp \
    logicnode.cpp \
    featurenode.cpp \
    treenode.cpp \
    fuzzydatatype.cpp \
    fuzzyobjectproperty.cpp \
    classrelatedfeaturenode.cpp \
    andnode.cpp \
    ornode.cpp \
    superclassnode.cpp \
    helperfunctions.cpp \
    ontologyparser.cpp \
    ontologyclass.cpp \
    ontologycontainer.cpp \
    ontologydata.cpp \
    complementnode.cpp \
    machinelearning/CDBN/DBN.cpp \
    machinelearning/CDBN/RBM.cpp \
    machinelearning/CDBN/CRBM.cpp \
    machinelearning/CDBN/HiddenLayer.cpp \
    machinelearning/CDBN/functions.cpp \
    machinelearning/CDBN/CDBN.cpp \
    machinelearning/CDBN/LogisticRegression.cpp \
    machinelearning/machinelearningdatatype.cpp \
    machinelearningnode.cpp \
    fuzzyevaluator.cpp

OTHER_FILES += \
    machinelearning/CDBN/BoostCDBN.pro.user

HEADERS += \
    fuzzyfunctions.h \
    featurenode.h \
    logicnode.h \
    treenode.h \
    fuzzydatatype.h \
    fuzzyobjectproperty.h \
    classrelatedfeaturenode.h \
    andnode.h \
    ornode.h \
    superclassnode.h \
    helperfunctions.h \
    ontologyparser.h \
    ontologyclass.h \
    ontologycontainer.h \
    ontologydata.h \
    complementnode.h \
    machinelearningnode.h \
    machinelearning/CDBN/RBM.h \
    machinelearning/CDBN/functions.h \
    machinelearning/CDBN/CDBN.h \
    machinelearning/CDBN/LogisticRegression.h \
    machinelearning/CDBN/HiddenLayer.h \
    machinelearning/CDBN/DBN.h \
    machinelearning/CDBN/CRBM.h \
    machinelearning/machinelearningdatatype.h \
    fuzzyevaluator.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lxml2
else:symbian: LIBS += -lxml2
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lxml2

INCLUDEPATH += $$PWD/../../../../../../../usr/include/libxml2
DEPENDPATH += $$PWD/../../../../../../../usr/include/libxml2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lpq
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lpq
else:symbian: LIBS += -lpq
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lpq

INCLUDEPATH += $$PWD/../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lpqxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lpqxx
else:symbian: LIBS += -lpqxx
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lpqxx

INCLUDEPATH += $$PWD/../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/release/ -lgomp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/lib/debug/ -lgomp
else:unix: LIBS += -L$$PWD/../../../../../../../usr/lib/ -lgomp

INCLUDEPATH += $$PWD/../../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../../usr/include

SUBDIRS += \
    machinelearning/CDBN/BoostCDBN.pro

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/lib/release/ -lxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/lib/debug/ -lxml2
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/lib/ -lxml2

INCLUDEPATH += $$PWD/../../../../../../../../usr/include/libxml2
DEPENDPATH += $$PWD/../../../../../../../../usr/include/libxml2
