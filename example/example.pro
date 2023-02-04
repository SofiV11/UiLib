include(../uilib.pri)

QT += core gui
CONFIG += c++11

TEMPLATE = app

DESTDIR = ../bin

CONFIG(debug, debug|release) {
    LIBS += -L$$OUT_PWD/../lib/debug/ -llib

    win32 {
        PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/lib.$${QMAKE_EXTENSION_STATICLIB}
    }
    unix{
        PRE_TARGETDEPS += $$OUT_PWD/../lib/debug/liblib.$${QMAKE_EXTENSION_STATICLIB}
    }
}

CONFIG(release, debug|release) {
    LIBS += -L$$OUT_PWD/../lib/release/ -llib
    win32 {
        PRE_TARGETDEPS += $$OUT_PWD/../lib/release/lib.$${QMAKE_EXTENSION_STATICLIB}
    }
    unix{
        PRE_TARGETDEPS += $$OUT_PWD/../lib/release/liblib.$${QMAKE_EXTENSION_STATICLIB}
    }
}

SOURCES += \
   src/main.cpp \
   src/Dialog.cpp

HEADERS += \
   src/Dialog.h

RESOURCES += \
   examplestyle.qrc
