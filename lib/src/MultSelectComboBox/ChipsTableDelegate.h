#pragma once

#include <QStyledItemDelegate>
#include <QApplication>

class ChipsTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ChipsTableDelegate( QObject* parent = 0 );

    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
    QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
    void setEditorData( QWidget* editor, const QModelIndex& index ) const override;
    void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const override;
    void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option,
                               const QModelIndex& index ) const override;

    void SetSelectAllBtn( const QString& selectAllBtnText );

signals:
    void SelectAllClicked( bool ) const;
    void OkClicked();

private slots:
    void OnEditorDataChanged();
    void OnOkDataChanged();

private:
    QString m_selectAllBtnText;
};



