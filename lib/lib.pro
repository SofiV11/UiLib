include(../uilib.pri)

TEMPLATE = lib
CONFIG += static

CONFIG += c++11

INCLUDEPATH += ../themes/interfaces

contains(QMAKE_HOST.arch, x86) {
        CONFIG(debug, debug|release) {
        OBJECTS_DIR = tmp/debug32
        DESTDIR = debug32
    }
    CONFIG(release, debug|release) {
        OBJECTS_DIR = tmp/release32
        DESTDIR = release32
    }
}

contains(QMAKE_HOST.arch, x86_64) {
    CONFIG(release, debug|release) {
        OBJECTS_DIR = tmp/release
        DESTDIR = release
    }
    CONFIG(debug, debug|release) {
        OBJECTS_DIR = tmp/debug
        DESTDIR = debug
    }
}


SOURCES += \
   src/CircleProgressBar.cpp \
   src/MultSelectComboBox/ChipsTableDelegate.cpp \
   src/MultSelectComboBox/ChipsTableModel.cpp \
   src/MultSelectComboBox/MultipleSelectComboBox.cpp \
   src/MultSelectComboBox/MultipleSelectComboBoxImpl.cpp \
   src/ToggleCheckBox.cpp \
   src/UiLibrary.cpp

HEADERS += \
   src/CircleProgressBar.h \
   src/MultSelectComboBox/ChipsTableDelegate.h \
   src/MultSelectComboBox/ChipsTableModel.h \
   src/MultSelectComboBox/MultipleSelectComboBox.h \
   src/MultSelectComboBox/MultipleSelectComboBoxImpl.h \
   src/ToggleCheckBox.h \
   src/UiLibrary.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
