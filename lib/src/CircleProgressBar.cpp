#include "CircleProgressBar.h"

CircleProgressBar::CircleProgressBar( QWidget* parent ) : QWidget( parent ),
    m_outherDiameter( 0 ),
    m_space( 0 ),
    m_innerDiameter( 0 ),
    m_min( 0 ),
    m_max( 1 ),
    m_nullPosition( 90 ),
    m_futureStepColor( 68, 133, 223 ),
    m_currentStepColor( 104, 158, 232 ),
    m_passedStepColor( 255, 255, 255 ),
    m_textColor( 255, 255, 255 )
{
    setObjectName( "circleProgressBar" );
    setFixedSize( 100, 100 );
    m_value = m_min;
}

int CircleProgressBar::currentStep() const
{
    return m_value;
}

int CircleProgressBar::stepsCount() const
{
    return m_max;
}

void CircleProgressBar::setStepsCount( int stepsCount )
{
    if ( stepsCount > m_min && stepsCount != m_max )
    {
        m_max = stepsCount;
        emit stepsCountChanged( true );
        m_value = m_min;
        update();
    }
}

void CircleProgressBar::setCurrentStep( int currentStep )
{
    if ( currentStep <= m_max && currentStep >= m_min && currentStep != m_value )
    {
        m_value = currentStep;
        emit currentStepChanged( m_value );
        update();
    }
}

void CircleProgressBar::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event )

    const double scale = 2 * devicePixelRatioF();
    QRectF m_baseRect( 1, 1, m_outherDiameter - 2, m_outherDiameter - 2 );
    QRectF m_innerRect( m_space, m_space, m_innerDiameter, m_innerDiameter );
    QImage image( m_outherDiameter * scale, m_outherDiameter * scale, QImage::Format_ARGB32 );
    image.fill( Qt::transparent );
    QPainter circlePainter( &image );

    circlePainter.scale( scale, scale );
    circlePainter.setRenderHint( QPainter::Antialiasing );

    QPainterPath dataPath;
    dataPath.moveTo( m_baseRect.center() );
    dataPath.addEllipse( m_baseRect );
    dataPath.addEllipse( m_innerRect );
    circlePainter.setPen( QPen( m_futureStepColor, 1 ) );
    circlePainter.setBrush( m_futureStepColor );
    circlePainter.drawPath( dataPath );

    double arcStep = 360.0 / double( m_max ) * m_value;
    double step = 360.0 / double( m_max );

    drawCircleSector( circlePainter, m_baseRect, arcStep + step, m_innerRect, m_futureStepColor );

    drawCircleSector( circlePainter, m_baseRect, arcStep, m_innerRect, m_currentStepColor );

    if ( m_value > m_min + 1 )
        drawCircleSector( circlePainter, m_baseRect, arcStep - step, m_innerRect, m_passedStepColor );

    circlePainter.setBrush( QColor( 0, 0, 0, 0 ) );
    circlePainter.drawEllipse( m_innerRect );

    drawText( circlePainter, m_innerRect, m_innerDiameter, m_value );
    circlePainter.end();

    QPainter painter( this );
    painter.setRenderHint( QPainter::SmoothPixmapTransform );
    painter.drawImage( event->rect(), image, image.rect() );
    painter.end();
}

void CircleProgressBar::drawCircleSector( QPainter& p, const QRectF& baseRect, double arcLength,
                                          const QRectF& innerRect, QColor brushColor )
{
    QPainterPath dataPath;
    dataPath.setFillRule( Qt::WindingFill );
    dataPath.moveTo( baseRect.center() );
    dataPath.arcTo( baseRect, m_nullPosition, -arcLength );

    QPointF currentPoint = dataPath.currentPosition ();
    currentPoint = baseRect.center () + ( ( currentPoint - baseRect.center () ) );
    dataPath.lineTo ( currentPoint );
    dataPath.moveTo ( baseRect.center () );
    qreal angle = m_nullPosition - arcLength;
    dataPath.arcTo( innerRect, angle, arcLength );
    currentPoint = dataPath.currentPosition ();
    currentPoint = baseRect.center () + ( ( currentPoint - baseRect.center () ) );
    dataPath.lineTo( currentPoint );

    p.setPen ( Qt :: NoPen );
    p.setBrush( brushColor );
    p.drawPath( dataPath );

}

void CircleProgressBar::drawText( QPainter& p, const QRectF& innerRect, int innerDiameter, double value )
{
    QFont f( font() );
    f.setFamily( m_textFontFamily );
    f.setPixelSize( innerDiameter * 0.3 );
    p.setFont( f );

    QRectF textRect = innerRect;
    p.setPen( m_textColor );
    p.drawText( textRect, Qt::AlignCenter, valueToText( value ) );
}

QString CircleProgressBar::valueToText( int value ) const
{

    QString textToDraw;

    if ( value && value <= ( m_max / ( m_max - m_min ) ) )
    {
        int text = m_max - ( m_max - value );
        textToDraw = QString::number( text, 'f', 0 ) + "/" + QString::number( m_max, 'f', 0 );
    }
    else if ( value && value >= ( m_max / ( m_max - m_min ) ) && value < m_max )
    {
        int text = m_max - ( m_max - value );
        textToDraw = QString::number( text, 'f', 0 ) + "/" + QString::number( m_max, 'f', 0 );
    }
    else if ( value && value == m_max )
    {
        int text = m_max;
        textToDraw = QString::number( text, 'f', 0 ) + "/" + QString::number( m_max, 'f', 0 );
    }
    else
        textToDraw = QString( "0/" ) + QString::number( m_max, 'f', 0 );

    return textToDraw;
}

void CircleProgressBar::resizeEvent( QResizeEvent* event )
{
    Q_UNUSED( event )
    const int wHeight = height();
    const int wWidth = width();
    m_outherDiameter = qMin( wWidth, wHeight );
    m_space = wHeight / 5;
    m_innerDiameter = m_outherDiameter - 2 * m_space;
}

void CircleProgressBar::goNextStep()
{
    if ( m_value < m_max )
    {
        m_value++;
        update();
    }
}

void CircleProgressBar::goPreviousStep()
{
    if ( m_value > m_min )
    {
        m_value--;
        update();
    }
}

QSize CircleProgressBar::minimumSizeHint() const
{
    return QSize( width(), height() );
}
