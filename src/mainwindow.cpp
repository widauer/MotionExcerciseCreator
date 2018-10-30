#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    calc = new movementCalculator();

    connect(ui->calculateButton, SIGNAL(clicked()), this, SLOT(calculateButtonClicked()));

    QLabel *aboutLabel = new QLabel();
    aboutLabel->setTextFormat(Qt::RichText);
    aboutLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    aboutLabel->setText("programmed with <a href=\"https://www.qt.io/\">QT 5.11</a> and <a href=\"https://www.qcustomplot.com/\">qcustomplot</a>");
    ui->statusBar->addPermanentWidget(aboutLabel);

    connect(aboutLabel, SIGNAL(linkActivated(const QString)), this, SLOT(labelLinkActivated(const QString)));

    calculateButtonClicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plotSummaryGraphs()
{
    ui->summaryVelocityPlot->clearGraphs();
    ui->summaryVelocityPlot->addGraph();
    ui->summaryVelocityPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getSummaryVelocityGraph());
    ui->summaryVelocityPlot->rescaleAxes();


    ui->summaryAccelerationPlot->clearGraphs();
    ui->summaryAccelerationPlot->addGraph();
    ui->summaryAccelerationPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getSummaryAccelerationGraph());
    ui->summaryAccelerationPlot->rescaleAxes();



    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(calc->Data.getSummaryMaxVelocity(), QString::number(calc->Data.getSummaryMaxVelocity(), 'g', 3));
    textTicker->addTick(0, "0");
    ui->summaryVelocityPlot->yAxis->setLabel("velocity in m/s");
    ui->summaryVelocityPlot->yAxis->setTicker(textTicker);


    QSharedPointer<QCPAxisTickerText> textTicker2(new QCPAxisTickerText);
    QVector<double> accelerationData = calc->Data.getSummaryAccelerationValues();
    textTicker2->addTick(accelerationData[0], QString::number(accelerationData[0], 'g', 3));
    textTicker2->addTick(accelerationData[1], QString::number(accelerationData[1], 'g', 3));
    textTicker2->addTick(0, "0");
    ui->summaryAccelerationPlot->yAxis->setLabel("acceleration in m/s²");
    ui->summaryAccelerationPlot->yAxis->setTicker(textTicker2);
    ui->summaryAccelerationPlot->yAxis->setRange(1.1 * accelerationData[1], 1.1 * accelerationData[0]);


    QSharedPointer<QCPAxisTickerText> textTicker3(new QCPAxisTickerText);
    QVector<double> timeData = calc->Data.getTimeValues();
    QVector<QString> timeLabels = calc->Data.getTimeLabels();
    for(int i = 0; i < timeData.length(); i++)
        textTicker3->addTick(timeData[i], timeLabels[i] + QString::number(timeData[i], 'g', 3));
    ui->summaryVelocityPlot->xAxis->setTicker(textTicker3);
    ui->summaryAccelerationPlot->xAxis->setTicker(textTicker3);
    ui->summaryVelocityPlot->xAxis->setRange(0, 1.02*timeData.last());
    ui->summaryAccelerationPlot->xAxis->setRange(0, 1.02*timeData.last());
    ui->summaryAccelerationPlot->xAxis->setLabel("time in s");


    ui->summaryVelocityPlot->replot();
    ui->summaryAccelerationPlot->replot();
}
void MainWindow::plotXAxisGraphs()
{
    ui->xPositionPlot->clearGraphs();
    ui->xPositionPlot->addGraph();
    ui->xPositionPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getXAxisPositionGraph());
    ui->xPositionPlot->rescaleAxes();


    ui->xVelocityPlot->clearGraphs();
    ui->xVelocityPlot->addGraph();
    ui->xVelocityPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getXAxisVelocityGraph());
    ui->xVelocityPlot->rescaleAxes();


    ui->xAccelerationPlot->clearGraphs();
    ui->xAccelerationPlot->addGraph();
    ui->xAccelerationPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getXAxisAccelerationGraph());
    ui->xAccelerationPlot->rescaleAxes();


    QSharedPointer<QCPAxisTickerText> textTicker1(new QCPAxisTickerText);
    QVector<double> positionData = calc->Data.getXAxisPositionValues();
    for(int i = 0; i < positionData.length(); i++) {
        if(positionData[i]*positionData[i] < 0.0001)
            positionData[i] = 0;
        textTicker1->addTick(positionData[i], QString::number(positionData[i], 'g', 3));
    }
    ui->xPositionPlot->yAxis->setLabel("position in mm");
    ui->xPositionPlot->yAxis->setTicker(textTicker1);
    double border = (positionData.last() - positionData.first()) * 0.02;
    ui->xPositionPlot->yAxis->setRange(positionData.first() - border, positionData.last() + border);



    QSharedPointer<QCPAxisTickerText> textTicker2(new QCPAxisTickerText);
    textTicker2->addTick(calc->Data.getXAxisMaxVelocity(), QString::number(calc->Data.getXAxisMaxVelocity(), 'g', 3));
    textTicker2->addTick(0, "0");
    ui->xVelocityPlot->yAxis->setLabel("velocity in m/s");
    ui->xVelocityPlot->yAxis->setTicker(textTicker2);
    ui->xVelocityPlot->yAxis->setRange(0, calc->Data.getXAxisMaxVelocity() * 1.02);



    QSharedPointer<QCPAxisTickerText> textTicker3(new QCPAxisTickerText);
    QVector<double> accelerationData = calc->Data.getXAxisAccelerationValues();
    textTicker3->addTick(accelerationData[0], QString::number(accelerationData[0], 'g', 3));
    textTicker3->addTick(accelerationData[1], QString::number(accelerationData[1], 'g', 3));
    textTicker3->addTick(0, "0");
    ui->xAccelerationPlot->yAxis->setLabel("acceleration in m/s²");
    ui->xAccelerationPlot->yAxis->setTicker(textTicker3);
    ui->xAccelerationPlot->yAxis->setRange(1.1 * accelerationData[1], 1.1 * accelerationData[0]);



    QSharedPointer<QCPAxisTickerText> textTicker4(new QCPAxisTickerText);
    QVector<double> timeData = calc->Data.getTimeValues();
    QVector<QString> timeLabels = calc->Data.getTimeLabels();
    for(int i = 0; i < timeData.length(); i++)
        textTicker4->addTick(timeData[i], timeLabels[i] + QString::number(timeData[i], 'g', 3));
    ui->xPositionPlot->xAxis->setTicker(textTicker4);
    ui->xVelocityPlot->xAxis->setTicker(textTicker4);
    ui->xAccelerationPlot->xAxis->setTicker(textTicker4);
    ui->xAccelerationPlot->xAxis->setLabel("time in s");

    ui->xPositionPlot->xAxis->setRange(0, 1.02*timeData.last());
    ui->xVelocityPlot->xAxis->setRange(0, 1.02*timeData.last());
    ui->xAccelerationPlot->xAxis->setRange(0, 1.02*timeData.last());

    ui->xPositionPlot->replot();
    ui->xVelocityPlot->replot();
    ui->xAccelerationPlot->replot();
}
void MainWindow::plotYAxisGraphs()
{
    ui->yPositionPlot->clearGraphs();
    ui->yPositionPlot->addGraph();
    ui->yPositionPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getYAxisPositionGraph());
    ui->yPositionPlot->rescaleAxes();


    ui->yVelocityPlot->clearGraphs();
    ui->yVelocityPlot->addGraph();
    ui->yVelocityPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getYAxisVelocityGraph());
    ui->yVelocityPlot->rescaleAxes();


    ui->yAccelerationPlot->clearGraphs();
    ui->yAccelerationPlot->addGraph();
    ui->yAccelerationPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getYAxisAccelerationGraph());
    ui->yAccelerationPlot->rescaleAxes();


    QSharedPointer<QCPAxisTickerText> textTicker1(new QCPAxisTickerText);
    QVector<double> positionData = calc->Data.getYAxisPositionValues();
    for(int i = 0; i < positionData.length(); i++) {
        if(positionData[i]*positionData[i] < 0.0001)
            positionData[i] = 0;
        textTicker1->addTick(positionData[i], QString::number(positionData[i], 'g', 3));
    }
    ui->yPositionPlot->yAxis->setLabel("position in mm");
    ui->yPositionPlot->yAxis->setTicker(textTicker1);
    double border = (positionData.last() - positionData.first()) * 0.02;
    ui->yPositionPlot->yAxis->setRange(positionData.first() - border, positionData.last() + border);



    QSharedPointer<QCPAxisTickerText> textTicker2(new QCPAxisTickerText);
    textTicker2->addTick(calc->Data.getYAxisMaxVelocity(), QString::number(calc->Data.getYAxisMaxVelocity(), 'g', 3));
    textTicker2->addTick(0, "0");
    ui->yVelocityPlot->yAxis->setLabel("velocity in m/s");
    ui->yVelocityPlot->yAxis->setTicker(textTicker2);
    ui->yVelocityPlot->yAxis->setRange(0, calc->Data.getYAxisMaxVelocity() * 1.02);



    QSharedPointer<QCPAxisTickerText> textTicker3(new QCPAxisTickerText);
    QVector<double> accelerationData = calc->Data.getYAxisAccelerationValues();
    textTicker3->addTick(accelerationData[0], QString::number(accelerationData[0], 'g', 3));
    textTicker3->addTick(accelerationData[1], QString::number(accelerationData[1], 'g', 3));
    textTicker3->addTick(0, "0");
    ui->yAccelerationPlot->yAxis->setLabel("acceleration in m/s²");
    ui->yAccelerationPlot->yAxis->setTicker(textTicker3);
    ui->yAccelerationPlot->yAxis->setRange(1.1 * accelerationData[1], 1.1 * accelerationData[0]);



    QSharedPointer<QCPAxisTickerText> textTicker4(new QCPAxisTickerText);
    QVector<double> timeData = calc->Data.getTimeValues();
    QVector<QString> timeLabels = calc->Data.getTimeLabels();
    for(int i = 0; i < timeData.length(); i++)
        textTicker4->addTick(timeData[i], timeLabels[i] + QString::number(timeData[i], 'g', 3));
    ui->yPositionPlot->xAxis->setTicker(textTicker4);
    ui->yVelocityPlot->xAxis->setTicker(textTicker4);
    ui->yAccelerationPlot->xAxis->setTicker(textTicker4);
    ui->yAccelerationPlot->xAxis->setLabel("time in s");

    ui->yPositionPlot->xAxis->setRange(0, 1.02*timeData.last());
    ui->yVelocityPlot->xAxis->setRange(0, 1.02*timeData.last());
    ui->yAccelerationPlot->xAxis->setRange(0, 1.02*timeData.last());

    ui->yPositionPlot->replot();
    ui->yVelocityPlot->replot();
    ui->yAccelerationPlot->replot();
}
void MainWindow::plotZAxisGraphs()
{
    ui->zPositionPlot->clearGraphs();
    ui->zPositionPlot->addGraph();
    ui->zPositionPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getZAxisPositionGraph());
    ui->zPositionPlot->rescaleAxes();


    ui->zVelocityPlot->clearGraphs();
    ui->zVelocityPlot->addGraph();
    ui->zVelocityPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getZAxisVelocityGraph());
    ui->zVelocityPlot->rescaleAxes();


    ui->zAccelerationPlot->clearGraphs();
    ui->zAccelerationPlot->addGraph();
    ui->zAccelerationPlot->graph(0)->addData(calc->Data.getTimeAxis(), calc->Data.getZAxisAccelerationGraph());
    ui->zAccelerationPlot->rescaleAxes();


    QSharedPointer<QCPAxisTickerText> textTicker1(new QCPAxisTickerText);
    QVector<double> positionData = calc->Data.getZAxisPositionValues();
    for(int i = 0; i < positionData.length(); i++) {
        if(positionData[i]*positionData[i] < 0.0001)
            positionData[i] = 0;
        textTicker1->addTick(positionData[i], QString::number(positionData[i], 'g', 3));
    }
    ui->zPositionPlot->yAxis->setLabel("position in mm");
    ui->zPositionPlot->yAxis->setTicker(textTicker1);
    double border = (positionData.last() - positionData.first()) * 0.02;
    ui->zPositionPlot->yAxis->setRange(positionData.first() - border, positionData.last() + border);



    QSharedPointer<QCPAxisTickerText> textTicker2(new QCPAxisTickerText);
    textTicker2->addTick(calc->Data.getZAxisMaxVelocity(), QString::number(calc->Data.getZAxisMaxVelocity(), 'g', 3));
    textTicker2->addTick(0, "0");
    ui->zVelocityPlot->yAxis->setLabel("velocity in m/s");
    ui->zVelocityPlot->yAxis->setTicker(textTicker2);
    ui->zVelocityPlot->yAxis->setRange(0, calc->Data.getZAxisMaxVelocity() * 1.02);



    QSharedPointer<QCPAxisTickerText> textTicker3(new QCPAxisTickerText);
    QVector<double> accelerationData = calc->Data.getZAxisAccelerationValues();
    textTicker3->addTick(accelerationData[0], QString::number(accelerationData[0], 'g', 3));
    textTicker3->addTick(accelerationData[1], QString::number(accelerationData[1], 'g', 3));
    textTicker3->addTick(0, "0");
    ui->zAccelerationPlot->yAxis->setLabel("acceleration in m/s²");
    ui->zAccelerationPlot->yAxis->setTicker(textTicker3);
    ui->zAccelerationPlot->yAxis->setRange(1.1 * accelerationData[1], 1.1 * accelerationData[0]);



    QSharedPointer<QCPAxisTickerText> textTicker4(new QCPAxisTickerText);
    QVector<double> timeData = calc->Data.getTimeValues();
    QVector<QString> timeLabels = calc->Data.getTimeLabels();
    for(int i = 0; i < timeData.length(); i++)
        textTicker4->addTick(timeData[i], timeLabels[i] + QString::number(timeData[i], 'g', 3));
    ui->zPositionPlot->xAxis->setTicker(textTicker4);
    ui->zVelocityPlot->xAxis->setTicker(textTicker4);
    ui->zAccelerationPlot->xAxis->setTicker(textTicker4);
    ui->zAccelerationPlot->xAxis->setLabel("time in s");

    ui->zPositionPlot->xAxis->setRange(0, 1.02*timeData.last());
    ui->zVelocityPlot->xAxis->setRange(0, 1.02*timeData.last());
    ui->zAccelerationPlot->xAxis->setRange(0, 1.02*timeData.last());

    ui->zPositionPlot->replot();
    ui->zVelocityPlot->replot();
    ui->zAccelerationPlot->replot();
}



void MainWindow::calculateButtonClicked()
{
    calc->setStartCoordinates(ui->xstartSpinBox->value(), ui->ystartSpinBox->value(), ui->zstartSpinBox->value());
    calc->setEndCoordinates(ui->xendSpinBox->value(), ui->yendSpinBox->value(), ui->zendSpinBox->value());
    calc->setMovementParameter(ui->velocitySpinBox->value(), ui->accelerationSpinBox->value(), ui->decelerationSpinBox->value());

    calc->calculateMovement();

    plotSummaryGraphs();
    plotXAxisGraphs();
    plotYAxisGraphs();
    plotZAxisGraphs();
}

void MainWindow::labelLinkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}
