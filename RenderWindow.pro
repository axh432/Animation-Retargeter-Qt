#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T11:35:39
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = RenderWindow
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS



SOURCES += \
        main.cpp \
        ui/mainwindow.cpp \
        ui/glwidget.cpp \
        engine/engine.cpp \
        engine/geometryengine.cpp \
        lib/TextProcessor/databuffer.cpp \
        lib/TextProcessor/textprocessor.cpp \
        lib/md5/md5animfactory.cpp \
        lib/md5/md5factory.cpp \
        lib/md5/md5modelfactory.cpp \
        lib/md5/mesh.cpp \
        lib/md5/quaternioncreator.cpp \
        lib/md5/skeleton.cpp \
    engine/camera.cpp \
    engine/entity.cpp \
    engine/material.cpp \
    engine/resourcemanager.cpp \
    lib/md5/glmesh.cpp


HEADERS += \
        ui/mainwindow.h \
        ui/glwidget.h \
        engine/engine.h \
        engine/geometryengine.h \
        lib/TextProcessor/databuffer.h \
        lib/TextProcessor/textprocessor.h \
        lib/md5/md5.h \
        lib/md5/md5animfactory.h \
        lib/md5/md5factory.h \
        lib/md5/md5modelfactory.h \
        lib/md5/quaternioncreator.h \
    engine/camera.h \
    engine/entity.h \
    engine/material.h \
    engine/resourcemanager.h \
    lib/md5/glmesh.h


RESOURCES += \
    resources/shaders.qrc \
    resources/textures.qrc
