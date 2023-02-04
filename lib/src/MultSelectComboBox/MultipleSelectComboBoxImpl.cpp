#include "MultipleSelectComboBoxImpl.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QStylePainter>
#include <QDebug>


MultipleSelectComboBoxImpl::MultipleSelectComboBoxImpl( bool showOkBtn, QWidget* parent )
    : QComboBox( parent )
    , m_chipsTableModel( new ChipsTableModel( this ) )
    , m_view( new QTableView( this ) )
    , m_displayText( "" )
    , m_showOkBtn( showOkBtn )
{
    setModel( m_chipsTableModel );
    setView( m_view );
    m_elements =
    {
        { NoSelectedPlaceHolder, "None"},
        {AllSelectedPlaceHolder, "Все модальности"},
        { SelectAllBtn, "Выбрать все"}
    };

    m_view->horizontalHeader()->hide();
    m_view->verticalHeader()->hide();
    m_view->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );

    m_chipsTableDelegate = new ChipsTableDelegate( this );
    m_chipsTableDelegate->SetSelectAllBtn( m_elements.value( SelectAllBtn ) );
    m_view->setItemDelegate( m_chipsTableDelegate );
    m_view->setEditTriggers( QAbstractItemView::CurrentChanged );
    m_view->viewport()->installEventFilter( this );
    setMouseTracking( true );

    connect( m_chipsTableDelegate, &ChipsTableDelegate::closeEditor, this,
             &MultipleSelectComboBoxImpl::OnUpdateDisplayText );
    connect( m_chipsTableDelegate, &ChipsTableDelegate::SelectAllClicked, this,
             &MultipleSelectComboBoxImpl::OnSelectAllClicked );
    connect( m_chipsTableDelegate, &ChipsTableDelegate::OkClicked, this, &MultipleSelectComboBoxImpl::OnOkClicked );
    connect( m_chipsTableModel, &ChipsTableModel::OpenPersistent, this, &MultipleSelectComboBoxImpl::OnOpenPersistent,
             Qt::QueuedConnection );
    connect( m_chipsTableModel, &ChipsTableModel::ClearTable, this, &MultipleSelectComboBoxImpl::OnUpdateDisplayText );
    connect( m_chipsTableModel, &QStandardItemModel::dataChanged, this, &MultipleSelectComboBoxImpl::OnUpdateDisplayText );
}

MultipleSelectComboBoxImpl::~MultipleSelectComboBoxImpl()
{
}

bool MultipleSelectComboBoxImpl::eventFilter( QObject* object, QEvent* event )
{
    // don't close items view after we release the mouse button
    // by simple eating MouseButtonRelease in viewport of items view
    if ( event->type() == QEvent::MouseButtonRelease && object == view()->viewport() )
        return true;

    return QComboBox::eventFilter( object, event );
}

void MultipleSelectComboBoxImpl::paintEvent( QPaintEvent* )
{
    QStylePainter painter( this );
    painter.setPen( palette().color( QPalette::Text ) );

    // draw the combobox frame, focus rect and selected etc.
    QStyleOptionComboBox opt;
    initStyleOption( &opt );

    opt.currentText = m_displayText;
    painter.drawComplexControl( QStyle::CC_ComboBox, opt );

    // draw the icon and text
    painter.drawControl( QStyle::CE_ComboBoxLabel, opt );
}

void MultipleSelectComboBoxImpl::SpanApply( const QPair<int, int>& coords, int btnWidth, int btnHight )
{
    const QModelIndex& index = m_chipsTableModel->index( coords.second, coords.first );
    // closePersistentEditor because it was opened at the signal OpenPersistent
    // from the model for single cell span
    m_view->closePersistentEditor( index );
    m_view->setSpan( coords.second, coords.first, btnHight, btnWidth );
    m_view->openPersistentEditor( index );
}

bool MultipleSelectComboBoxImpl::SelectAll()
{
    return SelectItem( m_elements.value( SelectAllBtn ), true );
}

void MultipleSelectComboBoxImpl::OnOpenPersistent( const QModelIndex& index )
{
    m_view->openPersistentEditor( index );
}

void MultipleSelectComboBoxImpl::AddItem ( const QString& itemName )
{
    int selectAllBtnSize = 2;
    QPair<int, int> coords;

    if ( m_showOkBtn )
    {
        m_chipsTableModel->RemoveLastRow();
        m_chipsTableModel->AddItem( itemName );
        m_view->clearSpans(); // single cell span for all except of the latest

        coords = m_chipsTableModel->AddOkSelectAll( m_elements.value( SelectAllBtn ) );
    }
    else
    {
        m_chipsTableModel->DeleteLastItem();
        m_chipsTableModel->AddItem( itemName );
        m_view->clearSpans();

        coords = m_chipsTableModel->AddItem( m_elements.value( SelectAllBtn ), selectAllBtnSize );
    }

    SpanApply( coords, selectAllBtnSize );
}

void MultipleSelectComboBoxImpl::RemoveItem( const QString& name )
{
    if ( IsEmpty() )
        return;

    QPair<int, int> coords;
    int selectAllBtnSize = 2;

    if ( m_showOkBtn )
    {
        m_chipsTableModel->RemoveLastRow();
        m_chipsTableModel->RemoveItem( name );
        coords = m_chipsTableModel->AddOkSelectAll( m_elements.value( SelectAllBtn ) );
    }
    else
    {
        m_chipsTableModel->DeleteLastItem();
        coords = m_chipsTableModel->RemoveItem( name );

        if ( coords.first == -1 || coords.second == -1 )
            return;

        coords = m_chipsTableModel->AddItem( m_elements.value( SelectAllBtn ), selectAllBtnSize );
    }

    SpanApply( coords, selectAllBtnSize );
}

void MultipleSelectComboBoxImpl::Clear()
{
    m_chipsTableModel->Clear();
}

QStringList MultipleSelectComboBoxImpl::GetItems() const
{
    QStringList itemsList;
    QVector<QStandardItem*> items = m_chipsTableModel->GetItems();

    for ( const QStandardItem* item : qAsConst( items ) )
        itemsList.push_back( item->data( Qt::DisplayRole ).toString() );

    return itemsList;
}

bool MultipleSelectComboBoxImpl::SelectItem( const QString& name, bool select )
{
    QVector<QStandardItem*> items = m_chipsTableModel->GetItems();

    if ( items.isEmpty() )
        return false;

    bool selected = false;

    for ( const QStandardItem* item : qAsConst( items ) )
    {
        if ( item->data( Qt::DisplayRole ).toString() == name )
        {
            m_chipsTableModel->SetData( item->index(), select );
            selected = true;
        }
    }

    if ( name != m_elements.value( SelectAllBtn ) )
        RemoveFromSelectedItemsByDistance( !select, -1 );
    else
        OnSelectAllClicked( select );

    return selected;
}

bool MultipleSelectComboBoxImpl::SelectItems( const QStringList& names, bool select )
{
    bool selected = false;

    for ( const QString& name : qAsConst( names ) )
        selected |= SelectItem( name, select );

    return selected;
}

QStringList MultipleSelectComboBoxImpl::GetSelectedItems() const
{
    QStringList activeItems;
    QVector<QStandardItem*> items = m_chipsTableModel->GetItems();

    for ( QStandardItem* item : qAsConst( items ) )
        if ( item->data( Qt::CheckStateRole ).toBool() )
            activeItems.push_back( item->data( Qt::DisplayRole ).toString() );

    return activeItems;
}

bool MultipleSelectComboBoxImpl::IsEmpty()
{
    QVector<QStandardItem*> items = m_chipsTableModel->GetItems();

    if ( items.isEmpty() )
        return true;

    return false;
}

void MultipleSelectComboBoxImpl::SetComboBoxElementsText( ComboBoxElements elements, const QString& text )
{
    m_elements[elements] = text;
    m_chipsTableDelegate->SetSelectAllBtn( m_elements.value( SelectAllBtn ) );
}

void MultipleSelectComboBoxImpl::RemoveFromSelectedItemsByDistance( bool state, int distance )
{
    const QVector<QStandardItem*>& items = m_chipsTableModel->GetItems();
    auto iter = items.end();
    std::advance( iter, distance );
    QStandardItem* item = *iter;
    m_chipsTableModel->SetData( item->index(), state );
    m_chipsTableModel->GetItems().removeOne( item );
}

void MultipleSelectComboBoxImpl::OnSelectAllClicked( bool state )
{
    const QVector<QStandardItem*>& items = m_chipsTableModel->GetItems();

    if ( state )
    {
        for ( int i = 0; i + 1 < items.size(); i++ )
            m_chipsTableModel->SetData( items.at( i )->index() );
    }
    else if ( items.size() >= 1 )
        RemoveFromSelectedItemsByDistance( state, -1 );
}

void MultipleSelectComboBoxImpl::OnOkClicked()
{
    hidePopup();
}

int MultipleSelectComboBoxImpl::OnUpdateDisplayText()
{
    QStringList activeItems = GetSelectedItems();
    QString displayText = activeItems.join( "," );

    QVector<QStandardItem*> items = m_chipsTableModel->GetItems();

    int interfaceCountBtns = ( m_showOkBtn ) ? 2 : 1;

    if ( activeItems.contains( m_elements.value( SelectAllBtn ) ) ||
            activeItems.count() == ( items.count() - interfaceCountBtns ) )
        displayText = m_elements.value( AllSelectedPlaceHolder );

    if ( displayText.length() == 0 )
        displayText = m_elements.value( NoSelectedPlaceHolder );

    m_displayText = displayText;
    update();

    return 0;
}
