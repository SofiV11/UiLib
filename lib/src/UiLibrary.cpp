#include "UiLibrary.h"
#include "../themes/interfaces/AppThemeInterface.h"

QMap<QString, QString> UiLibrary::m_themePathMap;
QString UiLibrary::m_themePath( "themeplugins/" );

QVector<UiLibrary::Theme> UiLibrary::GetThemeList()
{
    m_themePathMap.clear();

    QVector<Theme> m_themeVector;
    QDir::SortFlags sortingRule = QDir::Name;
    QDir dir = QDir( m_themePath );
    QFileInfoList themesFileList = dir.entryInfoList( QDir::Files, sortingRule );

    QString keyId;
    QString currentPath;

    for ( const QFileInfo& fileInfo : themesFileList )
    {
        Theme theme;
        QPluginLoader loader;
        loader.setFileName( m_themePath + fileInfo.fileName() );
        AppThemeInterface* themePlugin = qobject_cast<AppThemeInterface*>( loader.instance() );

        if ( !themePlugin ) continue;

        theme.name = themePlugin->getName();
        theme.id = themePlugin->getId();
        m_themeVector.push_back( theme );

        keyId =  theme.id;
        currentPath = fileInfo.filePath();
        m_themePathMap.insert( keyId, currentPath );
    }

    return m_themeVector;
}

bool UiLibrary::ApplyTheme( QApplication* app, const QString& themeId )
{
    QPluginLoader loader;
    QMap <QString, QString>::Iterator iter = m_themePathMap.find( themeId );

    if ( iter != m_themePathMap.end() && iter.key() == themeId )
        loader.setFileName( iter.value() );

    AppThemeInterface* theme = qobject_cast<AppThemeInterface*>( loader.instance() );

    if ( theme )
    {
        theme->apply( app );
        return true;
    }
    else
    {
        qDebug() << loader.errorString();
        return false;
    }

}

void UiLibrary::SetThemePath( const QString& path )
{
    m_themePath =  path;
}

