#include "LightThemePlugin.h"

QString LightThemePlugin::getId() const
{
    return QString( "12133" );
}

QString LightThemePlugin::getName() const
{
    return "Light Theme";
}

bool LightThemePlugin::apply( QApplication* app ) const
{
    QFile file( ":/theme/resources/light-theme.css" );
    file.open( QFile::ReadOnly );

    if ( file.isOpen() )
    {
        QString styleSheet = QString( file.readAll() );
        app->setStyleSheet( styleSheet );
        return 1;
    }
    else
    {
        qDebug() << "Style file is not open";
        return 0;
    }
}
