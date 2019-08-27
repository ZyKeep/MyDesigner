CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(myuiplugin)
TEMPLATE    = lib

HEADERS     = slidenavigationplugin.h  myui.h\
    navlistviewplugin.h mybuttonbarplugin.h\
animationbuttonplugin.h lightpointplugin.h\
    colorprogressbarplugin.h  mygraphicsviewplugin.h\
qswitchbuttonplugin.h mynavtreeplugin.h \
mytimertableviewplugin.h\
    qnavigationwidgetplugin.h

SOURCES     = slidenavigationplugin.cpp  myui.cpp\
    navlistviewplugin.cpp   mybuttonbarplugin.cpp\
animationbuttonplugin.cpp lightpointplugin.cpp\
    colorprogressbarplugin.cpp mygraphicsviewplugin.cpp \
mytimertableviewplugin.cpp\
qswitchbuttonplugin.cpp mynavtreeplugin.cpp\
    qnavigationwidgetplugin.cpp

RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target


include(slidenavigation.pri)
include(colorprogressbar.pri)
include(navlistview.pri)
include(mybuttonbar.pri)
include(animationbutton.pri)
include(qnavigationwidget.pri)
include(lightpoint.pri)
include(mygraphicsview.pri)
include(qswitchbutton.pri)
include(mynavtree.pri)
include(mytimertableview.pri)

DISTFILES +=

