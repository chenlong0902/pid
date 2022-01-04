#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QButtonGroup>
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
    void InitUI();
    void InitChart();
    void InitPID(IncPID *IncPID, float Kp, float Ki, float Kd, float val_min, float val_max);
    float CalcPID(IncPID *IncPID);

    int m_timerID = 0;
    float m_curSpeed;
protected:
    virtual void timerEvent(QTimerEvent *e);
public slots:
    void on_ButtonGroup_changed(int id);
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void on_pushButton_reset_clicked();

private:
    int m_testID;
    qint64 timestampList[4];

    QSplineSeries *seriesList[4];
    IncPID IncPID_data;
    QButtonGroup *buttonGroup;
    QChart *m_chart;
    float m_timeRange;
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
