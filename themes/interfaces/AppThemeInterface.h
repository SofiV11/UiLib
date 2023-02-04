#ifndef APPTHEMEINTERFACE_H
#define APPTHEMEINTERFACE_H

#include <QObject>
#include <QApplication>

class AppThemeInterface
{
public:
    virtual ~AppThemeInterface() {}
    virtual QString getId() const = 0; // Возвращает уникальный идентификатор темы
    virtual QString getName() const = 0; // Возвращает название цветовой темы
    virtual bool apply( QApplication* app ) const = 0; // Устанавливает тему для объекта приложения
};

#define AppThemeInterface_iid "appthemeinterface"
Q_DECLARE_INTERFACE(AppThemeInterface, AppThemeInterface_iid)

#endif // APPTHEMEINTERFACE_H
