TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lglfw
LIBS += -lglut
LIBS += -lGLU
LIBS += -lGLEW
LIBS += -lGL
LIBS += -lm
LIBS += -lboost_system
LIBS += /usr/local/lib/libMagick++-7.Q16HDRI.so

LIBS += /usr/local/lib/libopencv_core.so
LIBS += /usr/local/lib/libopencv_imgproc.so
LIBS += /usr/local/lib/libopencv_highgui.so
LIBS += /usr/local/lib/libopencv_dnn.so
LIBS += /usr/local/lib/libopencv_imgcodecs.so

INCLUDEPATH += /usr/local/include/ImageMagick-7/

SOURCES += main.cpp \
    libobj.cpp \
    shader.cpp \
    texture.cpp \
    skybox/skybox.cpp

HEADERS += \
    libobj.h \
    shader.h \
    texture.h \
    skybox/skybox.h \
    skybox/camera.h

DISTFILES += \
    shaders/vertShader.glsl \
    shaders/fragShader.glsl \
    skybox/shaders/fragSkyBox.glsl \
    skybox/shaders/vertSkyBox.glsl

SUBDIRS += \
    skybox \

#include(skybox/skybox.pri)
