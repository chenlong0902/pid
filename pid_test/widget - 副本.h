#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <iostream>
#include "def.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


using namespace std;


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void PID_init();
    float PID_realize(float speed);

    PID pid;
    int m_timerID = 0;
    float m_curSpeed;
protected:
    virtual void timerEvent(QTimerEvent *e);
public slots:
    void on_pushButton_clicked();
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
