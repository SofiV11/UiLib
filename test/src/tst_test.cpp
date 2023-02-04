
#include "../lib/src/ToggleCheckBox.h"
#include "../lib/src/CircleProgressBar.h"
#include "../lib/src/MultSelectComboBox/MultipleSelectComboBox.h"

#include <QDebug>
class test : public QObject
{
    Q_OBJECT

public:
    test();
    ~test();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_toggleCheckBox();
    void test_circleProgressBar();
    void test_multSelectComboBox();

};

test::test()
{

}

test::~test()
{

}

void test::initTestCase()
{

}

void test::cleanupTestCase()
{

}

void test::test_toggleCheckBox()
{
    ToggleCheckBox checkBox;
    QVERIFY( checkBox.isChecked() == false );
    QSignalSpy spy( &checkBox, &ToggleCheckBox::stateChanged );
    checkBox.toggle();
    QCOMPARE( spy.count(), 1 );
    const QList<QVariant> arguments = spy.takeFirst();
    QVERIFY( arguments.at( 0 ).toInt() == Qt::Checked );
    QVERIFY( checkBox.isChecked() == true );
}

void test::test_circleProgressBar()
{
    CircleProgressBar progressBar;
    QVERIFY( progressBar.currentStep() == 0 );
    progressBar.goNextStep();
    QVERIFY( progressBar.currentStep() == 1 );
    progressBar.goPreviousStep();
    QVERIFY( progressBar.currentStep() == 0 );

    int max_value = progressBar.stepsCount();

    for ( int i = 0; i < max_value + 10; i++ )
        progressBar.goNextStep();

    QVERIFY( progressBar.currentStep() == max_value );

    progressBar.setCurrentStep( 0 );

    for ( int i = 0; i < 10; i++ )
        progressBar.goPreviousStep();

    QVERIFY( progressBar.currentStep() == 0 );

    QSignalSpy spyStepsCount( &progressBar, &CircleProgressBar::stepsCountChanged );
    progressBar.setStepsCount( 10 );
    QCOMPARE( spyStepsCount.count(), 1 );
    QList<QVariant> arguments_2 = spyStepsCount.takeFirst();
    QVERIFY( arguments_2.at( 0 ) == true );
    QVERIFY( progressBar.stepsCount() == 10 );

    QSignalSpy spyCurrentSteps( &progressBar, &CircleProgressBar::currentStepChanged );
    progressBar.setCurrentStep( 4 );
    QCOMPARE( spyCurrentSteps.count(), 1 );
    QList<QVariant> arguments = spyCurrentSteps.takeFirst();
    QVERIFY( arguments.at( 0 ).toInt() == 4 );
    QVERIFY( progressBar.currentStep() == 4 );

    progressBar.setCurrentStep( progressBar.stepsCount() + 10 );
    QVERIFY( progressBar.currentStep() <= progressBar.stepsCount() );

    progressBar.setCurrentStep( -1 );
    QVERIFY( progressBar.currentStep() >= 0 );
}

void test::test_multSelectComboBox()
{
    MultipleSelectComboBox multComboBox( false );
    QStringList expected = {"", "Выбрать все" };
    multComboBox.AddItem( "" );
    QCOMPARE( multComboBox.GetItems(), expected );
    multComboBox.RemoveItem( "" );
    QVERIFY( multComboBox.IsEmpty() );
    multComboBox.RemoveItem( "CT" );
    QVERIFY( multComboBox.IsEmpty() );

    multComboBox.AddItem( "CG" );
    QCOMPARE( multComboBox.SelectItem( "CR" ), false );
    QCOMPARE( multComboBox.SelectItems( {"CG", "MR"} ), true );
    QCOMPARE( multComboBox.SelectAll(), true );

    multComboBox.Clear();
    QCOMPARE( multComboBox.SelectAll(), false );
    QCOMPARE( multComboBox.SelectItem( "CG" ), false );
    QCOMPARE( multComboBox.SelectItems( {"CG", "AA", "Выбрать все" } ), false );

    multComboBox.AddItem( "CG" );
    multComboBox.AddItem( "RT" );
    multComboBox.SelectItem( "CG" );
    QStringList expectedSelection( "CG" );
    QCOMPARE( multComboBox.GetSelectedItems(), expectedSelection );
    multComboBox.SelectItems( {"CG", "RT"} );
    expectedSelection.append( "RT" );
    QCOMPARE( multComboBox.GetSelectedItems(), expectedSelection );
    QStringList expectedItems( {"CG", "RT", "Выбрать все" } );
    QCOMPARE( multComboBox.GetItems(), expectedItems );

    // test for combobox with OK
    MultipleSelectComboBox multComboBoxWithOk( true );
    QStringList list = {"ML", "OK", "Выбрать все" };

    multComboBoxWithOk.AddItem( "ML" );
    QCOMPARE( multComboBoxWithOk.GetItems(), list );
    multComboBoxWithOk.RemoveItem( "dsd" );
    QCOMPARE( multComboBoxWithOk.GetItems(), list );

    multComboBoxWithOk.Clear();
    QCOMPARE( multComboBoxWithOk.SelectAll(), false );
    QCOMPARE( multComboBoxWithOk.SelectItem( "some" ), false );

    multComboBoxWithOk.AddItem( "ML" );
    QCOMPARE( multComboBoxWithOk.GetItems(), list );
    multComboBoxWithOk.AddItem( "mri" );
    QCOMPARE( multComboBoxWithOk.SelectItems( {"ML", "mri"} ), true );
}

QTEST_MAIN( test )

#include "tst_test.moc"
