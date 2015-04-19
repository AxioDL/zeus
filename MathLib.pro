TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

gcc:QMAKE_CXXFLAGS += -msse -msse2 -msse3
msvc:QMAKE_CXXFLAGS += /arch:SSE2
win32:DEFINES -= WIN64

include (../Athena/AthenaCore.pri)
include (MathLib.pri)

SOURCES += main.cpp

