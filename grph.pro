TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \ 
    src/frame_buf.cpp \
    src/main.cpp \
    src/pipeline.cpp \
    src/geom.cpp

HEADERS += \ 
    src/frame_buf.h \
    src/pipeline.h \
    src/geom.h
