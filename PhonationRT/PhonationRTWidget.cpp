#include "PhonationRTWidget.h"

using namespace std;

PhonationRTWidget::PhonationRTWidget(QWidget *parent)
    : QChartView(parent)
{

    connect(&thread, SIGNAL(renderedImage(float,float,float,float)),
            this, SLOT(updateNumbers(float,float,float,float)));
    timer.start(20, this);

}

void PhonationRTWidget::updateNumbers(float upt_a, float upt_b, float upt_c, float upt_d)
{
    a = upt_a;
    b = upt_b;
    c = upt_c;
    d = upt_d;

    series1 = new QLineSeries();
    series2 = new QLineSeries();
    series3 = new QAreaSeries();

    chart()->removeAllSeries();

    series1->append(0,100);

    series1->append(-100, (200*a)-100);
    series1->append(-50, (200*a)-100);

    series1->append(-50, (200*b)-100);
    series1->append(0, (200*b)-100);

    series1->append(0, (200*c)-100);
    series1->append(50,(200*c)-100);

    series1->append(50,(200*d)-100);
    series1->append(100,(200*d)-100);

    series2->append(-100,-100);

    series3->setUpperSeries(series1);
    series3->setLowerSeries(series2);
    series3->setOpacity(0.5);

    chart()->addSeries(series1);
    chart()->addSeries(series2);
    chart()->addSeries(series3);

    for (QAbstractAxis *axis : axisList) {
        series1->attachAxis(axis);
        series2->attachAxis(axis);
        series3->attachAxis(axis);
    }
}

void PhonationRTWidget::timerEvent(QTimerEvent *event){
    if (event->timerId() == timer.timerId()) {
            thread.render();
        } else {
            QWidget::timerEvent(event);
        }
}
