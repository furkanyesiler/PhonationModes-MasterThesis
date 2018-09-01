#ifndef PHONATIONRTWIDGET_H
#define PHONATIONRTWIDGET_H

#include <QPixmap>
#include <QWidget>
#include <QChartView>
#include <QtCharts>
#include <QBasicTimer>
#include "PhonationRTProcess.h"

QT_CHARTS_USE_NAMESPACE

class PhonationRTWidget : public QChartView
{
    Q_OBJECT

public:
    PhonationRTWidget(QWidget *parent = 0);

protected:
    void timerEvent(QTimerEvent *event) override;

private slots:
    void updateNumbers(float upt_a, float upt_b, float upt_c, float upt_d);

private:

    PhonationRTProcess thread;
    QBasicTimer timer;
    const QList<QAbstractAxis *> axisList = chart()->axes();
    QLineSeries *series1;
    QLineSeries *series2;
    QAreaSeries *series3;
    float a;
    float b;
    float c;
    float d;
};

#endif // PHONATIONRTWIDGET_H
