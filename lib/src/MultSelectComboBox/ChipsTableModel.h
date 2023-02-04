#pragma once

#include <QStandardItemModel>

class ChipsTableModel: public QStandardItemModel
{
    Q_OBJECT
public:
    ChipsTableModel( QObject* parent = nullptr );
    virtual ~ChipsTableModel();

    QPair<int, int> AddItem( const QString& name, int btnWidth = 1 );
    QPair<int, int> AddOkSelectAll( const QString& selectAllText );

    QPair<int, int> RemoveItem( const QString& name );
    void RemoveLastRow();
    void DeleteLastItem();
    void Clear();

    void SetData( const QModelIndex& index, bool state = false );
    QVector<QStandardItem*> GetItems() const;
    int GetMaxColumnCount() const;

private:
    void UpdateLastItemRowColumn();

signals:
    void OpenPersistent( const QModelIndex& index );
    void ClearTable();

private:
    int m_lastItemRow, m_lastItemColumn;
    const int m_maxColumnCount;

};

