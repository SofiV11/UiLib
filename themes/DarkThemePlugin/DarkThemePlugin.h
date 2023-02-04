#ifndef DARKTHEMEPLUGIN_H
#define DARKTHEMEPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "../interfaces/AppThemeInterface.h"
#include <QFile>
#include <QtWidgets/QApplication>
#include <QString>

class DarkThemePlugin : public QObject, public AppThemeInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "com.inobitec.uilib.AppThemeInterface" )
    Q_INTERFACES( AppThemeInterface )

public:
    DarkThemePlugin() {};
    virtual ~DarkThemePlugin() {};

    // AppThemeInterface interface
    QString getId() const override;
    QString getName() const override;
    bool apply( QApplication* app ) const override;
};

#endif // DARKTHEMEPLUGIN_H
