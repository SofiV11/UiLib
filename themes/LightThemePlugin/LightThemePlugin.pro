QT += gui\
      widgets

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++11

DESTDIR = ../../bin/themeplugins

INCLUDEPATH += ../interfaces

SOURCES += \
    LightThemePlugin.cpp

HEADERS += \
    LightThemePlugin.h

DISTFILES += LightThemePlugin.json

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
