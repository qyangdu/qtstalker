TEMPLATE = lib
CONFIG += plugin
MOC_DIR += build
OBJECTS_DIR += build
INCLUDEPATH += ../../src
QMAKE_CXXFLAGS += -fPIC -O2


HEADERS += MarkerRetracement.h
SOURCES += MarkerRetracement.cpp
HEADERS += MarkerRetracementDialog.h
SOURCES += MarkerRetracementDialog.cpp
HEADERS += MarkerRetracementObject.h
SOURCES += MarkerRetracementObject.cpp
HEADERS += ../../src/ColorButton.h
#SOURCES += ../../src/ColorButton.cpp
HEADERS += ../../src/Object.h
#SOURCES += ../../src/Object.cpp
HEADERS += ../../src/Util.h
#SOURCES += ../../src/Util.cpp
HEADERS += ../../src/Plugin.h
HEADERS += ../../src/PluginCommand.h
HEADERS += ../../src/ObjectCommand.h
#SOURCES += ../../src/ObjectCommand.cpp
HEADERS += ../../src/Dialog.h
#SOURCES += ../../src/Dialog.cpp

OBJECTS += ../../src/build/ColorButton.o
OBJECTS += ../../src/build/Dialog.o
OBJECTS += ../../src/build/Object.o
OBJECTS += ../../src/build/ObjectCommand.o
OBJECTS += ../../src/build/Util.o


HOME=$$system(echo $HOME) 
target.path =  $${HOME}/OTA/lib
INSTALLS += target

QT += core
QT += gui