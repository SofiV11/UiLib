#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include "MultipleSelectComboBoxImpl.h"

/*
 * Class wrapper for MultipleSelectComboBoxImpl to restrict access to method addItem of parent QComboBox
*/

class MultipleSelectComboBox: public QWidget
{
public:
    explicit MultipleSelectComboBox( bool showOk = true, QWidget* parent = nullptr );

    void AddItem( const QString& name ); // Добавить элемент
    void RemoveItem( const QString& name ); // Удалить элемент по имени
    void Clear(); // Удалить все элементы
    QStringList GetItems() const; // Получить имена всех элементов

    bool SelectItem( const QString& name,
                     bool select = true ); // Выделить или снять выделение с элемента по имени
    bool SelectAll();
    bool SelectItems( const QList<QString>& names,
                      bool select = true ); // Выделить или снять выделение с элементов по имени
    QStringList GetSelectedItems() const; // Получить имена всех выделенных элементов

    // Установить текст, для элементов комбобокса
    void SetComboBoxElementsText( MultipleSelectComboBoxImpl:: ComboBoxElements elements,
                                  const QString& text );
    bool IsEmpty();// Проверка на пустоту таблицы

private:
    MultipleSelectComboBoxImpl* m_multSelectComboBoxImpl;
    QVBoxLayout* m_layout;
};
