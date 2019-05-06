CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(myuiplugin)
TEMPLATE    = lib

HEADERS     = slidenavigationplugin.h  myui.h\
               treenavigationbarplugin.h \
    colorprogressbarplugin.h
SOURCES     = slidenavigationplugin.cpp  myui.cpp\
               treenavigationbarplugin.cpp \
    colorprogressbarplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(treenavigationbar.pri)
include(slidenavigation.pri)
include(colorprogressbar.pri)
