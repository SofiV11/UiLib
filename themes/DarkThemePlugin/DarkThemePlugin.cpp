#include "DarkThemePlugin.h"
#include <QDebug>

QString DarkThemePlugin::getId() const
{
    return QString( "23434" );
}

QString DarkThemePlugin::getName() const
{
    return "Dark Theme";
}

bool DarkThemePlugin::apply( QApplication* app ) const
{
    QFile file( ":/theme/resources/dark-theme.css" );
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
