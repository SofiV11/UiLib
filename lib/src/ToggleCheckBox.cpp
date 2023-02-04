#include "ToggleCheckBox.h"

ToggleCheckBox::ToggleCheckBox( QWidget* parent ) : QCheckBox( parent ),
    m_handleAnimation( new QVariantAnimation( this ) ),
    m_handleX( 0 ),
    m_handleY( 0 ),
    m_backgroundRadius( 0 ),
    m_handleDiameter( 0 ),
    m_margin( 0 ),
    m_endValue( 0 ),
    m_isHovered( false ),
    m_colorRectEnabled( 104, 109, 115 ),
    m_colorRectDisabled( 221, 223, 226 ),
    m_borderColor( m_colorRectDisabled ),
    m_handleColorEnabled( 240, 241, 243 ),
    m_handleColorDiasbled( 104, 109, 115 ),
    m_colorRectEnabledHover( 151, 156, 162 ),
    m_colorRectDisabledHover( 151, 156, 162 ),
    m_borderColorHover( 151, 156, 162 ),
    m_handleColorEnabledHover( 62, 65, 70 ),
    m_handleColorDiasbledHover( 62, 65, 70 )
{
    if ( this->isCheckable() ) setObjectName( "toggleCheckBox" );
    else setObjectName( "toggleCheckBoxUncheckable" );

    setFixedSize( 50, 30 );
    connect( this, &QCheckBox::clicked, this, &ToggleCheckBox::onToggleCheckBoxClicked );
    connect( m_handleAnimation, &QVariantAnimation::valueChanged, this, &ToggleCheckBox::updateHandlePosition );
}

void ToggleCheckBox::enterEvent( QEvent* event )
{
    Q_UNUSED( event )
    m_isHovered = true;
    update();
}

void ToggleCheckBox::leaveEvent( QEvent* event )
{
    Q_UNUSED( event )
    m_isHovered = false;
    update();
}

void ToggleCheckBox::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event )

    const double scale = 2 * devicePixelRatioF();
    QImage image( size() * scale, QImage::Format_ARGB32 );
    image.fill( Qt::transparent );
    QPainter painter( &image );
    painter.scale( scale, scale );
    painter.setRenderHint( QPainter::Antialiasing );

    if ( isChecked() ) //checked
    {
        drawBackground( painter, m_colorRectEnabled, m_colorRectEnabled );
        drawHandle( painter, m_handleColorEnabled, m_handleColorEnabled );

        if ( m_isHovered && isEnabled() )
        {
            drawBackground( painter, m_colorRectEnabledHover, m_colorRectEnabledHover );
            drawHandle( painter, m_handleColorEnabledHover, m_handleColorEnabledHover );
        }
    }
    else  //unchecked
    {
        drawBackground( painter, m_borderColor, m_colorRectDisabled );
        drawHandle( painter, m_handleColorDiasbled, m_handleColorDiasbled );

        if ( m_isHovered && isEnabled() )
        {
            drawBackground( painter, m_borderColorHover, m_colorRectDisabledHover );
            drawHandle( painter, m_handleColorDiasbledHover, m_handleColorDiasbledHover );
        }
    }

    QPainter widgetPainter( this );
    widgetPainter.setRenderHint( QPainter::SmoothPixmapTransform );
    widgetPainter.drawImage( event->rect(), image, image.rect() );
    widgetPainter.end();
}

void ToggleCheckBox::resizeEvent( QResizeEvent* event )
{
    Q_UNUSED( event )
    const int wHeight = height();
    m_backgroundRadius = ( wHeight - 4 ) / 2;
    m_handleDiameter = ( wHeight / 1.66 );
    m_margin = ( wHeight - m_handleDiameter ) / 2;
    m_handleY = m_margin;
    m_handleX = m_margin;
}

bool ToggleCheckBox::hitButton( const QPoint& pos ) const
{
    return QAbstractButton::hitButton( pos );
}

void ToggleCheckBox::onToggleCheckBoxClicked()
{
    if ( isChecked() ) //->
    {
        m_handleAnimation->setStartValue( m_handleX );
        m_handleAnimation->setDuration( 100 );
        m_handleAnimation->setEndValue( width() - height() + m_margin );
    }
    else //<-
    {
        m_handleAnimation->setStartValue( m_handleX );
        m_handleAnimation->setDuration( 100 );
        m_handleAnimation->setEndValue( m_margin );
    }

    m_handleAnimation->start();
}

void ToggleCheckBox::updateHandlePosition()
{
    m_handleX = m_handleAnimation->currentValue().toInt();
    update();
}

void ToggleCheckBox::drawBackground( QPainter& painter, const QColor& borderColor, const QColor& fillColor )
{
    painter.setPen( borderColor );
    painter.drawRoundedRect( QRectF( 1, 1, width() - 2, height() - 2 ), m_backgroundRadius, m_backgroundRadius,
                             Qt::AbsoluteSize );
    painter.setBrush( fillColor );
    painter.drawRoundedRect( QRectF( 1, 1, width() - 2, height() - 2 ), m_backgroundRadius, m_backgroundRadius,
                             Qt::AbsoluteSize );
}

void ToggleCheckBox::drawHandle( QPainter& painter, const QColor& borderColor, const QColor& fillColor )
{
    painter.setPen( QPen( borderColor, 1, Qt::SolidLine ) );
    painter.drawEllipse( QRectF( m_handleX, m_handleY, m_handleDiameter, m_handleDiameter ) );
    painter.setBrush( fillColor );
    painter.drawEllipse( QRectF( m_handleX, m_handleY, m_handleDiameter, m_handleDiameter ) );
}
