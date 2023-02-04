#ifndef CIRCLEPROGRESSBAR_H
#define CIRCLEPROGRESSBAR_H

class CircleProgressBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QColor futureStepColor MEMBER m_futureStepColor )
    Q_PROPERTY( QColor currentStepColor MEMBER m_currentStepColor )
    Q_PROPERTY( QColor passedStepColor MEMBER m_passedStepColor )
    Q_PROPERTY( QColor textColor MEMBER m_textColor )
    Q_PROPERTY ( QString textFontFamily MEMBER m_textFontFamily )
public:
    explicit CircleProgressBar( QWidget* parent = nullptr );
    int currentStep() const;
    int stepsCount() const;

public slots:
    void goNextStep();
    void goPreviousStep();
    void setStepsCount( int stepsCount );
    void setCurrentStep( int currentStep );

signals:
    void currentStepChanged( int step );
    bool stepsCountChanged( bool isChanged );

private:
    void paintEvent( QPaintEvent* event ) override;
    QSize minimumSizeHint() const override;
    void drawCircleSector( QPainter& p, const QRectF& baseRect, double arcLength, const QRectF& innerRect,
                           QColor brushColor );
    void drawText( QPainter& p, const QRectF& m_innerRect, int m_innerDiameter, double value );
    QString valueToText( int value ) const;
    void resizeEvent( QResizeEvent* event ) override;

private:
    int m_outherDiameter;
    int m_space;
    int m_innerDiameter;

    int m_min, m_max;
    int m_value;
    int m_nullPosition;

    QRectF m_baseRect;
    QRectF m_innerRect;

    QColor m_futureStepColor;
    QColor m_currentStepColor;
    QColor m_passedStepColor;
    QColor m_textColor;

    QString m_textFontFamily;
};

#endif // CIRCLEPROGRESSBAR_H
