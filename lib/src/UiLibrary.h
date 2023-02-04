#ifndef UILIBRARY_H
#define UILIBRARY_H



class UiLibrary
{
public:
    struct Theme
    {
        QString id;
        QString name;
    };
    QVector<Theme> static GetThemeList();
    void static SetThemePath ( const QString& path );
    bool static ApplyTheme( QApplication* app, const QString& themeId );
private:
    static QMap<QString, QString> m_themePathMap;
    static QString m_themePath;
};

#endif // UILIBRARY_H
