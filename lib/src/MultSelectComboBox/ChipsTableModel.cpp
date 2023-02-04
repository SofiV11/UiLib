#include "ChipsTableModel.h"
#include <stdexcept>
#include <QPair>

ChipsTableModel::ChipsTableModel( QObject* parent )
    : QStandardItemModel( parent )
    , m_lastItemRow( -1 )
    , m_lastItemColumn( -1 )
    , m_maxColumnCount( 3 )
{
    setColumnCount( 0 );
    setRowCount( 0 );
}

ChipsTableModel::~ChipsTableModel()
{
}

QVector<QStandardItem*> ChipsTableModel::GetItems() const
{
    QVector<QStandardItem*> items;

    for ( int row = 0; row < rowCount(); row++ )
    {
        for ( int col = 0; col < columnCount(); col++ )
        {
            QStandardItem* it = item( row, col );

            if ( it == nullptr )
                break;

            items.append( it );
        }
    }

    return items;
}

int ChipsTableModel::GetMaxColumnCount() const
{
    return m_maxColumnCount;
}

void ChipsTableModel::UpdateLastItemRowColumn()
{
    for ( int i = m_lastItemRow; i > -1; --i )
    {
        for ( int j = m_maxColumnCount - 1; j > -1; --j )
        {
            if ( item( i, j ) != nullptr )
            {
                m_lastItemRow = i;
                m_lastItemColumn = j;
                return;
            }
        }
    }

    m_lastItemRow = -1;
    m_lastItemColumn = -1;
}

void ChipsTableModel::DeleteLastItem()
{
    if ( m_lastItemColumn > -1 && m_lastItemRow > -1 )
    {
        QStandardItem* selectall = takeItem( m_lastItemRow, m_lastItemColumn );
        delete selectall;
        UpdateLastItemRowColumn();
        setRowCount( m_lastItemRow + 1 );
    }
}

void ChipsTableModel::Clear()
{
    m_lastItemRow = -1;
    m_lastItemColumn = -1;
    clear();
    emit ClearTable();
}

QPair<int, int> ChipsTableModel::AddItem( const QString& name, int btnWidth )
{
    if ( btnWidth > m_maxColumnCount )
        throw std::runtime_error( "btnWidth is greater than m_maxColumnCount" );

    int rows = rowCount();
    int columns = columnCount();
    int x = m_lastItemColumn + 1;
    int y = m_lastItemRow;

    if ( rows == 0 && columns == 0 )
    {
        setColumnCount( btnWidth );
        setRowCount( 1 );
        y = x = 0;
    }
    else if ( rows == 1 && columns < m_maxColumnCount )
        setColumnCount( columns + 1 );
    else if ( ( ( columns - 1 ) - m_lastItemColumn ) < btnWidth )
    {
        setRowCount( rows + 1 );
        y += 1;
        x = 0;
    }

    setItem( y, x, new QStandardItem( name ) );
    emit OpenPersistent( index( y, x ) );

    m_lastItemColumn = x;
    m_lastItemRow = y;

    return {m_lastItemColumn, m_lastItemRow};
}

QPair<int, int> ChipsTableModel::RemoveItem( const QString& name )
{
    const QVector<QStandardItem*>& items = GetItems();
    bool found = false;

    for ( const QStandardItem* item : items )
    {
        if ( name == item->data( Qt::DisplayRole ).toString() )
        {
            QStandardItem* it = takeItem( item->index().row(), item->index().column() );
            found = true;
            delete it;
        }
    }

    if ( !found )
        return {-1, -1};

    int rows = rowCount();

    int cols = columnCount();

    QVector<QStandardItem*> newItems;

    for ( int row = 0; row < rows; row++ )
    {
        for ( int col = 0; col < cols ; col++ )
        {
            QStandardItem* it = takeItem( row, col );

            if ( it == nullptr )
                break;

            newItems.append( it );
        }
    }

    Clear();

    QPair<int, int> addedItem;
    int size = newItems.size();

    for ( int i = 0; i < size; i++ )
    {
        const QStandardItem* it = newItems.at( i );

        if ( it == nullptr )
            break;

        addedItem = AddItem( it->data( Qt::DisplayRole ).toString() );
        m_lastItemColumn = addedItem.first;
        m_lastItemRow = addedItem.second;
        SetData( index( m_lastItemRow, m_lastItemColumn ), it->data( Qt::CheckStateRole ).toBool() );
    }

    return {m_lastItemColumn, m_lastItemRow};
}

void ChipsTableModel::RemoveLastRow()
{
    if ( m_lastItemColumn > -1 && m_lastItemRow > -1 )
    {
        removeRow( m_lastItemRow );
        m_lastItemRow --;
        UpdateLastItemRowColumn();
        setRowCount( m_lastItemRow + 1 );
    }
}

QPair<int, int> ChipsTableModel::AddOkSelectAll( const QString& selectAllBtnText )
{
    int col = 0;
    int row = m_lastItemRow + 1;
    QStandardItem* item = new QStandardItem( "OK" );
    setItem( row, col, item );
    item->setData( true, Qt::UserRole );

    setItem( row, ++col, new QStandardItem( selectAllBtnText ) );
    emit OpenPersistent( index( row, col ) );

    m_lastItemColumn = col;
    m_lastItemRow++;

    return {m_lastItemColumn, m_lastItemRow};
}

void ChipsTableModel::SetData( const QModelIndex& index, bool state )
{
    QMap<int, QVariant> data = itemData( index );
    data[Qt::DisplayRole] = index.data();
    data[Qt::CheckStateRole] = state;
    setItemData( index, data );

    emit dataChanged( index, index );
}

