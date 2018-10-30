#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <movementcalculator.h>
#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    movementCalculator     *calc;

    void        plotSummaryGraphs();
    void        plotXAxisGraphs();
    void        plotYAxisGraphs();
    void        plotZAxisGraphs();

public slots:
    void        calculateButtonClicked();

    void        labelLinkActivated(const QString &link);
};

#endif // MAINWINDOW_H
