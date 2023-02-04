#ifndef LIGHTTHEMEPLUGIN_H
#define LIGHTTHEMEPLUGIN_H

#include <QObject>
#include <QDebug>
#include <QtPlugin>
#include "../interfaces/AppThemeInterface.h"
#include <QFile>
#include <QtWidgets/QApplication>
#include <QString>

class LightThemePlugin : public QObject, AppThemeInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "com.inobitec.uilib.AppThemeInterface" )
    Q_INTERFACES( AppThemeInterface )

public:
    LightThemePlugin() {};
    ~LightThemePlugin() {};

    // AppThemeInterface interface
    QString getId() const override;
    QString getName() const override;
    bool apply( QApplication* app ) const override;
};

#endif // LIGHTTHEMEPLUGIN_H

