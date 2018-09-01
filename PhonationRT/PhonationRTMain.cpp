#include "PhonationRTWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const qreal angularMin = -100;
    const qreal angularMax = 100;

    const qreal radialMin = -100;
    const qreal radialMax = 100;

    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    QAreaSeries *series3 = new QAreaSeries();

    series3->setUpperSeries(series1);
    series3->setLowerSeries(series2);
    series3->setOpacity(0.5);

    series1->append(-100, 100);
    series1->append(-50,100);

    series1->append(-50, 100);
    series1->append(0,100);

    series1->append(0, 100);
    series1->append(50,100);

    series1->append(50,100);
    series1->append(-100,100);

    series2->append(-100,-100);

    QPolarChart *chart = new QPolarChart();

    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->addSeries(series3);
    chart->legend()->setVisible(false);

    chart->setContentsMargins(0,40,0,0);

    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(1);
    angularAxis->setLabelsVisible(false);
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(5);
    radialAxis->setLabelFormat("%d");
    radialAxis->setLabelsVisible(false);
    radialAxis->setLineVisible(false);
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);

    series1->attachAxis(radialAxis);
    series1->attachAxis(angularAxis);
    series2->attachAxis(radialAxis);
    series2->attachAxis(angularAxis);
    series3->attachAxis(radialAxis);
    series3->attachAxis(angularAxis);

    radialAxis->setRange(radialMin, radialMax);
    angularAxis->setRange(angularMin, angularMax);

    PhonationRTWidget *chartView = new PhonationRTWidget();
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.setFixedHeight(600);
    window.setFixedWidth(800);
    window.setWindowTitle("PhonationRT");
    QLabel *labelT = new QLabel(&window);
    labelT->setText("Real Time Automatic Classification of Phonation Modes in Singing");
    QFont* titleFont = new QFont("Times", 14, QFont::Bold);
    labelT->setFont(*titleFont);
    labelT->setGeometry(0,0,550,25);
    labelT->move((window.width()-550)/2,13);
    QLabel *labelB = new QLabel(&window);
    labelB->setText("Breathy");
    labelB->move(150,130);
    QLabel *labelF = new QLabel(&window);
    labelF->setText("Flow");
    labelF->move(600,130);
    QLabel *labelP = new QLabel(&window);
    labelP->setText("Pressed");
    labelP->move(600,470);
    QLabel *labelN = new QLabel(&window);
    labelN->setText("Neutral");
    labelN->move(150,470);
    //window.resize(800, 600);

    window.show();

    return app.exec();
}
