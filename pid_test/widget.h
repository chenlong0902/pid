#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <iostream>
#include "def.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


using namespace std;
using namespace QtCharts;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void InitChart();
    void InitPID(IncPID *IncPID, float Kp, float Ki, float Kd, float val_min, float val_max);
    float CalcPID(IncPID *IncPID);

    int m_timerID = 0;
    float m_curSpeed;
protected:
    virtual void timerEvent(QTimerEvent *e);
public slots:
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void on_pushButton_reset_clicked();

private:
    qint64 timestamp;
    QSplineSeries *m_series;
    IncPID IncPID_data;

    QChart *m_chart;
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
