QT += gui\
      widgets

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++11

#CONFIG(debug, debug|release){
#    BUILDTYPE = build-debug
#}
#else {
#    BUILDTYPE = build-release
#}

#DESTDIR = ../../$${BUILDTYPE}/themes/styleplugins

DESTDIR = ../../bin/themeplugins

INCLUDEPATH += ../interfaces

SOURCES += \
    DarkThemePlugin.cpp

HEADERS += \
    DarkThemePlugin.h

DISTFILES += DarkThemePlugin.json

unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
