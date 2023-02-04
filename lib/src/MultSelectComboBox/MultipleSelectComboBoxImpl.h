#pragma once

#include <QComboBox>
#include <QTableView>

#include "ChipsTableDelegate.h"
#include "ChipsTableModel.h"

class MultipleSelectComboBoxImpl: public QComboBox
{
    Q_OBJECT

public:

    enum ComboBoxElements
    {
        NoSelectedPlaceHolder = 0,
        AllSelectedPlaceHolder,
        SelectAllBtn
    };

    explicit MultipleSelectComboBoxImpl( bool showOkBtn, QWidget* parent = nullptr );
    virtual ~MultipleSelectComboBoxImpl();
    void paintEvent( QPaintEvent* ) override;
    bool eventFilter( QObject* object, QEvent* event ) override;

    void AddItem( const QString& itemName );
    void RemoveItem( const QString& name );
    void Clear();
    QStringList GetItems() const;

    bool SelectAll();
    bool SelectItem( const QString& name, bool select = true );
    bool SelectItems( const QStringList& names, bool select = true );
    QStringList GetSelectedItems() const;
    bool IsEmpty();

    void SetComboBoxElementsText( ComboBoxElements elements, const QString& text );

    void SpanApply( const QPair<int, int>& coords, int btnWidth = 1, int btnHight = 1 );

    /**
     * The method removes an element from items and selects or deselects the given element.
     * The method is intended not to add the Ok and SelectAll buttons to the general list of items
     *
     * @param state sets the state of the selected or not element
     * @param distance is the position in items.
     * If the numbering is from the end, then the value is negative, otherwise it is positive
     */
    void RemoveFromSelectedItemsByDistance( bool state, int distance );

private slots:
    void OnOpenPersistent( const QModelIndex& index );
    int OnUpdateDisplayText();
    void OnSelectAllClicked( bool state );
    void OnOkClicked();

private:
    ChipsTableModel* m_chipsTableModel;
    QTableView* m_view;
    ChipsTableDelegate* m_chipsTableDelegate;
    QMap<ComboBoxElements, QString> m_elements;
    QString m_displayText;
    bool m_showOkBtn;

};
