#ifndef TOGGLECHECKBOX_H
#define TOGGLECHECKBOX_H

class ToggleCheckBox : public QCheckBox
{
    Q_OBJECT

    Q_PROPERTY( bool enabled READ isEnabled )
    Q_PROPERTY( QColor roundedRectColorEnabled MEMBER m_colorRectEnabled )
    Q_PROPERTY( QColor roundedRectColorDisabled MEMBER m_colorRectDisabled )
    Q_PROPERTY( QColor roundedRectColorDisabledBorder MEMBER m_borderColor )
    Q_PROPERTY( QColor handleColorEnabled MEMBER m_handleColorEnabled )
    Q_PROPERTY( QColor handleColorDisabled MEMBER m_handleColorDiasbled )
    Q_PROPERTY( QColor colorRectEnabledHover MEMBER m_colorRectEnabledHover )
    Q_PROPERTY( QColor colorRectDisabledHover MEMBER m_colorRectDisabledHover )
    Q_PROPERTY( QColor borderHover MEMBER m_borderColorHover )
    Q_PROPERTY( QColor handleColorEnabledHover MEMBER m_handleColorEnabledHover )
    Q_PROPERTY( QColor handleColorDiasbledHover MEMBER m_handleColorDiasbledHover )

public:
    ToggleCheckBox( QWidget* parent = nullptr );

protected:
    void enterEvent( QEvent* event ) override;
    void leaveEvent( QEvent* event ) override;
    void paintEvent( QPaintEvent* event ) override;
    void resizeEvent( QResizeEvent* event ) override;
    bool hitButton( const QPoint& pos ) const override;

private slots:
    void onToggleCheckBoxClicked();
    void updateHandlePosition();

private:
    void drawBackground( QPainter& painter, const QColor& borderColor, const QColor& fillColor );
    void drawHandle( QPainter& painter, const QColor& borderColor, const QColor& fillColor );

private:
    QVariantAnimation* m_handleAnimation;

    int m_handleX;
    int m_handleY;
    int m_backgroundRadius;
    int m_handleDiameter;
    int m_margin;

    int m_endValue;
    bool m_isHovered;

    QColor m_colorRectEnabled;
    QColor m_colorRectDisabled;
    QColor m_borderColor;
    QColor m_handleColorEnabled;
    QColor m_handleColorDiasbled;

    QColor m_colorRectEnabledHover;
    QColor m_colorRectDisabledHover;
    QColor m_borderColorHover;
    QColor m_handleColorEnabledHover;
    QColor m_handleColorDiasbledHover;
};

#endif // TOGGLECHECKBOX_H
