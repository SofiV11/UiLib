#include "ChipsTableModel.h"
#include "ChipsTableDelegate.h"

#include <QStyle>
#include <QPushButton>
#include <QStylePainter>

ChipsTableDelegate::ChipsTableDelegate( QObject* parent ) :
    QStyledItemDelegate( parent )
{
}

void ChipsTableDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option,
                                const QModelIndex& index ) const
{
    bool value = index.data( Qt::CheckStateRole ).toBool();
    QString text = index.data( Qt::DisplayRole ).toString();

    const QStyle* style = QApplication::style();
    QStyleOptionButton opt;
    opt.state |= value ? QStyle::State_On : QStyle::State_Off;
    opt.state |= QStyle::State_Enabled;
    opt.text = text;
    opt.rect = option.rect;

    style->drawControl( QStyle::CE_PushButton, &opt, painter, option.widget );
}

QWidget* ChipsTableDelegate::createEditor( QWidget* parent,
                                           const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Q_UNUSED( option )
    bool isOk = index.data( Qt::UserRole ).toBool();
    QPushButton* editor = new QPushButton( parent );
    editor->setObjectName( "Chips" );
    editor->setCheckable( !isOk );

    if ( isOk )
        connect( editor, &QPushButton::clicked, this, &ChipsTableDelegate::OnOkDataChanged );
    else
        connect( editor, &QPushButton::clicked, this, &ChipsTableDelegate::OnEditorDataChanged );

    return editor;
}

void ChipsTableDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
    QPushButton* myEditor = qobject_cast<QPushButton*>( editor );
    Q_CHECK_PTR( myEditor );

    if ( index.data().isNull() )
    {
        myEditor->setProperty( "emptyitem", true );
        return;
    }

    myEditor->setObjectName( "Chips" );
    myEditor->setText( index.data( Qt::DisplayRole ).toString() );
    myEditor->setChecked( index.data( Qt::CheckStateRole ).toBool() );
}

void ChipsTableDelegate::setModelData( QWidget* editor, QAbstractItemModel* model,
                                       const QModelIndex& index ) const
{
    QPushButton* myEditor = qobject_cast<QPushButton*>( editor );
    Q_CHECK_PTR( myEditor );

    if ( index.data().isNull() )
    {
        myEditor->setCheckable( false );
        return;
    }

    ChipsTableModel* myModel = qobject_cast<ChipsTableModel*>( model );
    Q_CHECK_PTR( myModel );
    bool value = myEditor->isChecked();

    QMap<int, QVariant> data = myModel->itemData( index );
    data[Qt::CheckStateRole] = value;
    data[Qt::DisplayRole] = myEditor->text();
    myModel->setItemData( index, data );

    if ( value )
        emit SelectAllClicked( m_selectAllBtnText == index.data().toString() ? true : false );
}

void ChipsTableDelegate::updateEditorGeometry( QWidget* editor,
                                               const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Q_UNUSED( index )
    editor->setGeometry( option.rect );
}

void ChipsTableDelegate::OnEditorDataChanged()
{
    QWidget* editor = qobject_cast<QWidget*>( sender() );

    if ( editor )
        emit commitData( editor );
}

void ChipsTableDelegate::OnOkDataChanged()
{
    QWidget* editor = qobject_cast<QWidget*>( sender() );

    if ( editor )
        emit commitData( editor );

    emit OkClicked();
}

void ChipsTableDelegate::SetSelectAllBtn( const QString& selectAllBtnText )
{
    m_selectAllBtnText = selectAllBtnText;
}
