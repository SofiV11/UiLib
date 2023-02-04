#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "../lib/src/ToggleCheckBox.h"
#include "../lib/src/CircleProgressBar.h"
#include "../lib/src/MultSelectComboBox/MultipleSelectComboBox.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog( QWidget* parent = nullptr );
    ~Dialog();

public slots:
    void AddItem();
    void RemoveItem();
    void SelectItem();
    void SelectAll();
    void SelectItems();
    void ClearItems();
    void onThemeChanged( const QString& idd );

private:
    void CreateMultComboBox();

private:
    ToggleCheckBox* m_toggleCheckBoxOnLight;
    ToggleCheckBox* m_toggleCheckBoxOffLight;
    ToggleCheckBox* m_toggleCheckBoxOnDark;
    ToggleCheckBox* m_toggleCheckBoxOffDark;
    QFrame* m_imageFrame;
    CircleProgressBar* m_progressBar;
    QPushButton* m_backButton;
    QPushButton* m_nextButton;
    QSpinBox* m_stepsCountSpinBox;
    QWidget* m_widgetComboBox;
    MultipleSelectComboBox* m_multSelectComboBox;
    QPushButton* m_addItemBtn;
    QPushButton* m_removeItemBtn;
    QPushButton* m_selectItemBtn;
    QPushButton* m_selectItemsBtn;
    QPushButton* m_clearBtn;
    QPushButton* m_selectAllBtn;
    QComboBox* m_themesComboBox;
    QString currentThemeName;
    QString currentIndexTheme;
};
#endif // DIALOG_H

