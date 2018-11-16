TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \ 
    src/frame_buf.cpp \
    src/main.cpp \
    src/pipeline.cpp \
    src/geom.cpp \
    src/tga_t.cpp \
    src/mouse.cpp \
    src/sphere.cpp \
    src/keyboard.cpp

HEADERS += \ 
    src/frame_buf.h \
    src/pipeline.h \
    src/geom.h \
    src/tga_t.h \
    src/mouse.h \
    src/sphere.h \
    src/keyboard.h

DISTFILES +=
