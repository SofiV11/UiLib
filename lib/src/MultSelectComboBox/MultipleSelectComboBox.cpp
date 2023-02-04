#include "MultipleSelectComboBox.h"
#include "MultipleSelectComboBoxImpl.h"

MultipleSelectComboBox::MultipleSelectComboBox( bool showOk, QWidget* parent ): QWidget( parent )
{
    m_multSelectComboBoxImpl = new MultipleSelectComboBoxImpl( showOk, parent ) ;
    m_multSelectComboBoxImpl->setObjectName( "Chips" );
    m_layout = new QVBoxLayout( parent );
    m_layout->addWidget( m_multSelectComboBoxImpl );
    setLayout( m_layout );
}

void MultipleSelectComboBox::AddItem( const QString& name )
{
    m_multSelectComboBoxImpl->AddItem( name );
}

void MultipleSelectComboBox::RemoveItem( const QString& name )
{
    m_multSelectComboBoxImpl->RemoveItem( name );
}

void MultipleSelectComboBox::Clear()
{
    m_multSelectComboBoxImpl->Clear();
}

QStringList MultipleSelectComboBox::GetItems() const
{
    return m_multSelectComboBoxImpl->GetItems();
}

bool MultipleSelectComboBox::SelectItem( const QString& name, bool select )
{
    return m_multSelectComboBoxImpl->SelectItem( name, select );
}

bool MultipleSelectComboBox::SelectAll()
{
    return m_multSelectComboBoxImpl->SelectAll();
}

bool MultipleSelectComboBox::SelectItems( const QList<QString>& names, bool select )
{
    return m_multSelectComboBoxImpl->SelectItems( names, select );
}

QStringList MultipleSelectComboBox::GetSelectedItems() const
{
    return m_multSelectComboBoxImpl->GetSelectedItems();
}

void MultipleSelectComboBox::SetComboBoxElementsText( MultipleSelectComboBoxImpl::ComboBoxElements elements,
                                                      const QString& text )
{
    m_multSelectComboBoxImpl->SetComboBoxElementsText( elements, text );
}

bool MultipleSelectComboBox::IsEmpty()
{
    return m_multSelectComboBoxImpl->IsEmpty();
}
