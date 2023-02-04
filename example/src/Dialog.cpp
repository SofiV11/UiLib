#include "Dialog.h"
#include <QDebug>

#include "../lib/src/UiLibrary.h"

Dialog::Dialog( QWidget* parent )
    : QDialog( parent )
{
    QFile file( ":/example/css/examplestyle.css" );
    file.open( QFile::ReadOnly );

    if ( file.isOpen() )
    {
        QString styleSheet = QString( file.readAll() );
        setStyleSheet( styleSheet );
    }
    else
        qDebug() << "Style file is not open";

    int margin = 30;
    int spacing = margin;

    m_toggleCheckBoxOnLight = new ToggleCheckBox( this );
    m_toggleCheckBoxOnLight->setObjectName( "toggleCheckBox" );

    m_toggleCheckBoxOffLight = new ToggleCheckBox( this );
    m_toggleCheckBoxOffLight->setObjectName( "toggleCheckBoxUncheckable" );
    m_toggleCheckBoxOffLight->setEnabled( false );

    m_toggleCheckBoxOnDark = new ToggleCheckBox( this );
    m_toggleCheckBoxOnDark->setObjectName( "toggleCheckBoxOnDark" );

    m_toggleCheckBoxOffDark = new ToggleCheckBox( this );
    m_toggleCheckBoxOffDark->setObjectName( "toggleCheckBoxOffDark" );
    m_toggleCheckBoxOffDark->setEnabled( false );

    QFrame* backgroundFrame = new QFrame( this );
    backgroundFrame->setObjectName( "backgroundFrame" );
    backgroundFrame->setGeometry( 0, 0, width(), height() );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( m_toggleCheckBoxOnLight, Qt::AlignCenter );
    layout->addWidget( m_toggleCheckBoxOffLight, Qt::AlignCenter );
    layout->addWidget( m_toggleCheckBoxOnDark, Qt::AlignCenter );
    layout->addWidget( m_toggleCheckBoxOffDark, Qt::AlignCenter );
    layout->setMargin( margin );
    layout->setSpacing( spacing );
    backgroundFrame->setLayout( layout );

    m_imageFrame = new QFrame();
    QVBoxLayout* layout2 = new QVBoxLayout( );
    m_progressBar = new CircleProgressBar( this );
    m_progressBar->setObjectName( "circleProgressBar" );
    m_backButton = new QPushButton( "back", this );
    m_backButton->setObjectName( "backButton" );
    m_nextButton = new QPushButton( "next", this );
    m_nextButton->setObjectName( "nextButton" );
    m_stepsCountSpinBox = new QSpinBox( this );
    m_stepsCountSpinBox->setRange( 1, 100 );
    connect( m_backButton, &QPushButton::clicked, m_progressBar, &CircleProgressBar::goPreviousStep );
    connect( m_nextButton, &QPushButton::clicked, m_progressBar, &CircleProgressBar::goNextStep );
    connect( m_stepsCountSpinBox, static_cast<void ( QSpinBox::* )( int )>( &QSpinBox::valueChanged ),
             m_progressBar, &CircleProgressBar::setStepsCount );

    int checkBoxLayoutWidth( m_toggleCheckBoxOffDark->width() + 2 * margin );
    int frameHeight( 4 * m_toggleCheckBoxOffDark->height() + 5 * margin );
    int frameWidth( m_progressBar->width() + 2 * margin );

    m_imageFrame->setGeometry( checkBoxLayoutWidth, 0, frameWidth, frameHeight );
    m_imageFrame->setObjectName( "imageFrame" );

    QHBoxLayout* h_layout = new QHBoxLayout();
    h_layout->addWidget( m_backButton );
    h_layout->addWidget( m_nextButton );

    QHBoxLayout* h_spinLayout = new QHBoxLayout();
    h_spinLayout->addItem( new QSpacerItem( margin, 0 ) );
    h_spinLayout->addWidget( m_stepsCountSpinBox );
    h_spinLayout->addItem( new QSpacerItem( margin, 0 ) );

    QHBoxLayout* h2_layout = new QHBoxLayout();
    h2_layout->addItem( new QSpacerItem( margin, 0 ) );
    h2_layout->addWidget( m_progressBar );
    h2_layout->addItem( new QSpacerItem( margin, 0 ) );

    layout2->addLayout( h_spinLayout );
    layout2->addLayout( h2_layout );
    layout2->addLayout( h_layout );

    m_imageFrame->setLayout( layout2 );

    CreateMultComboBox();

    QHBoxLayout* main_layout = new QHBoxLayout( this );
    main_layout->addWidget( backgroundFrame );
    main_layout->addWidget( m_imageFrame );
    main_layout->addWidget( m_widgetComboBox );
    main_layout->setContentsMargins( 0, 0, 0, 0 );
    main_layout->setSpacing( 0 );
    setLayout( main_layout );

    QHBoxLayout* combo_box_layout = new QHBoxLayout();
    m_themesComboBox = new QComboBox( this );
    combo_box_layout->addWidget( m_themesComboBox, Qt::AlignHCenter );
    combo_box_layout->setMargin( 10 );


    QVector <UiLibrary::Theme> vector;
    UiLibrary::SetThemePath( QString( "themeplugins/" ) );
    vector.append( UiLibrary::GetThemeList() );
    UiLibrary::ApplyTheme( qApp,  vector[0].id );

    for ( int i = 0; i < vector.size(); i++ )
    {
        QString name = vector[i].name;
        QString id = vector[i].id;
        m_themesComboBox->addItem( name, QVariant( id ) );
    }

    currentThemeName = m_themesComboBox->currentText();
    currentIndexTheme = m_themesComboBox->currentData().toString();
    main_layout->addLayout( combo_box_layout );

    connect( m_themesComboBox, static_cast<void ( QComboBox::* )( const QString& )>( &QComboBox::currentTextChanged ),
             this, &Dialog::onThemeChanged );
}

Dialog::~Dialog()
{
}

void Dialog::CreateMultComboBox()
{
    m_widgetComboBox = new QWidget( this );
    m_widgetComboBox->setMinimumSize( 250, 300 );
    m_multSelectComboBox = new MultipleSelectComboBox( true, this );
    m_multSelectComboBox->setObjectName( "Chips" );
    m_multSelectComboBox->SetComboBoxElementsText( MultipleSelectComboBoxImpl::ComboBoxElements::AllSelectedPlaceHolder,
                                                   "Все модальности" );

    QStringList list = {"CR", "CT", "DX",
                        "ECG", "ES", "EPS",
                        "OT", "PT", "SC",
                        "MG", "MR", "NM",
                        "SR", "US", "XA", "XC"
                       };

    m_multSelectComboBox->SetComboBoxElementsText( MultipleSelectComboBoxImpl::ComboBoxElements::SelectAllBtn,
                                                   "SELECT ALL" );

    for ( const QString& name : list )
        m_multSelectComboBox->AddItem( name );

    m_addItemBtn = new QPushButton( "Add item", this );
    m_removeItemBtn = new QPushButton( "Remove Item DX", this );
    m_selectItemBtn = new QPushButton( "Select Item MR", this );
    m_selectItemsBtn = new QPushButton( "OT, PT, SC, XA", this );
    m_clearBtn = new QPushButton( "Clear", this );
    m_selectAllBtn = new QPushButton( "Select All", this );

    QVBoxLayout* vLayout = new QVBoxLayout( this );
    vLayout->addWidget( m_multSelectComboBox );
    vLayout->addWidget( m_addItemBtn );
    vLayout->addWidget( m_removeItemBtn );
    vLayout->addWidget( m_selectItemBtn );
    vLayout->addWidget( m_selectAllBtn );
    vLayout->addWidget( m_selectItemsBtn );
    vLayout->addWidget( m_clearBtn );

    m_widgetComboBox->setLayout( vLayout );

    connect( m_addItemBtn, &QPushButton::clicked, this, &Dialog::AddItem );
    connect( m_removeItemBtn, &QPushButton::clicked, this, &Dialog::RemoveItem );
    connect( m_selectItemBtn, &QPushButton::clicked, this, &Dialog::SelectItem );
    connect( m_selectAllBtn, &QPushButton::clicked, this, &Dialog::SelectAll );
    connect( m_selectItemsBtn, &QPushButton::clicked, this, &Dialog::SelectItems );
    connect( m_clearBtn, &QPushButton::clicked, this, &Dialog::ClearItems );
}

void Dialog::AddItem()
{
    QStringList list = {"CR", "CT", "DX",
                        "ECG", "ES", "EPS",
                        "OT", "PT", "SC",
                        "MG", "MR", "NM",
                        "SR", "US", "XA", "XC"
                       };

    static int i = 0;

    const QString& text = list.at( i++ );
    m_multSelectComboBox->AddItem( text );
}

void Dialog::RemoveItem()
{
    m_multSelectComboBox->RemoveItem( "DX" );
}

void Dialog::SelectItem()
{
    m_multSelectComboBox->SelectItem( "MR" );
}

void Dialog::SelectAll()
{
    m_multSelectComboBox->SelectAll();
}

void Dialog::SelectItems()
{
    m_multSelectComboBox->SelectItems( {"OT", "PT", "SC", "XA"} );
}

void Dialog::ClearItems()
{
    m_multSelectComboBox->Clear();
}

void Dialog::onThemeChanged( const QString& idd )
{
    Q_UNUSED( idd )
    currentIndexTheme = m_themesComboBox->currentData().toString();
    UiLibrary::ApplyTheme( qApp,  currentIndexTheme );
    update();
}

