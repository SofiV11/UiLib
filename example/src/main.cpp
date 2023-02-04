#include "Dialog.h"

#include <QApplication>
#include "../lib/src/UiLibrary.h"

int main( int argc, char* argv[] )
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy( Qt::HighDpiScaleFactorRoundingPolicy::PassThrough );
#endif
    QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
    QApplication a( argc, argv );
    Dialog w;
    w.show();
    return a.exec();
}
