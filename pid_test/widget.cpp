#include "widget.h"
#include "ui_widget.h"
#include <QDateTime>


#define MAXOUT 1000                 //输出最大值


Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget),
      timestamp(0)
{
    ui->setupUi(this);
    InitChart();
    InitPID(&IncPID_data, 0.15, 0.15, 0.00, 0.00, 50.00);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::InitChart(){
    m_series = new QSplineSeries();
    m_series->setName("spline");
    //![1]

    //![2]

    m_chart = new QChart();
    m_chart->legend()->show();
    m_chart->addSeries(m_series);
    m_chart->setTitle("Simple spline chart example");
    m_chart->createDefaultAxes();
    m_chart->axes(Qt::Vertical).first()->setRange(0, 40);
    m_chart->axes(Qt::Horizontal).first()->setRange(0, 10);

//    m_series->append(0,5);
//    m_series->append(5.5,10);
//    m_series->append(6.5,14);
//    m_series->append(7.5,17);
//    m_series->append(8.5,20);

//        m_series->append(0, 6);
//        m_series->append(2, 4);
//        m_series->append(3, 8);
//        m_series->append(7, 4);
//        m_series->append(10, 5);
//        *m_series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    //![4]
    QChartView *chartView = new QChartView(m_chart,this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setGeometry(20,20,780,350);
}

void Widget::InitPID(IncPID *IncPID, float Kp, float Ki, float Kd, float val_min, float val_max)
{
    IncPID->set_val = 0.00;
    IncPID->cul_val = 0.00;
    IncPID->out_val = 0.00;

    IncPID->val_min = val_min;
    IncPID->val_max = val_max;

    IncPID->Kp = Kp;
    IncPID->Ki = Ki;
    IncPID->Kd = Kd;

    IncPID->Kp_err = 0.0;
    IncPID->Ki_err = 0.0;
    IncPID->Kd_err = 0.0;

    IncPID->Ek = 0.0;
    IncPID->Ek_1 = 0.0;
    IncPID->Ek_2 = 0.0;

    ui->label_curspeed->setText(QString::number(0));
    ui->lineEdit_startspeed->setText(QString::number(0));
    ui->lineEdit_targetspeed->setText(QString::number(30));
    ui->lineEdit_p->setText(QString::number(Kp));
    ui->lineEdit_i->setText(QString::number(Ki));
    ui->lineEdit_d->setText(QString::number(Kd));
}

void Widget::on_pushButton_start_clicked(){
    ui->label_curspeed->setText(ui->lineEdit_startspeed->text());
    IncPID_data.cul_val = ui->lineEdit_startspeed->text().toFloat();
    IncPID_data.set_val = ui->lineEdit_targetspeed->text().toFloat();
    IncPID_data.Kp = ui->lineEdit_p->text().toFloat();
    IncPID_data.Ki = ui->lineEdit_i->text().toFloat();
    IncPID_data.Kd = ui->lineEdit_d->text().toFloat();
    m_timerID = startTimer(500);
}

void Widget::on_pushButton_stop_clicked(){
    if(m_timerID > 0){
        killTimer(m_timerID);
        m_timerID = 0;
        timestamp = 0;
    }
}

void Widget::on_pushButton_reset_clicked(){
    if(m_timerID > 0){
        killTimer(m_timerID);
        m_timerID = 0;
        timestamp = 0;
    }

    m_series->clear();
    ui->label_curspeed->setText(ui->lineEdit_startspeed->text());
    IncPID_data.cul_val = ui->lineEdit_startspeed->text().toFloat();
    IncPID_data.set_val = ui->lineEdit_targetspeed->text().toFloat();
    IncPID_data.Kp = ui->lineEdit_p->text().toFloat();
    IncPID_data.Ki = ui->lineEdit_i->text().toFloat();
    IncPID_data.Kd = ui->lineEdit_d->text().toFloat();
}

void Widget::timerEvent(QTimerEvent *e){
    if(m_timerID == e->timerId()){
        IncPID_data.cul_val += CalcPID(&IncPID_data);
        ui->label_curspeed->setText(QString::number(IncPID_data.cul_val));
        cout<<"速度为: "<<IncPID_data.cul_val<<endl;
        if(timestamp == 0){
            m_series->append(0,IncPID_data.cul_val);
            timestamp = QDateTime::currentMSecsSinceEpoch();
        }
        else{
            qint64 curtimestamp = QDateTime::currentMSecsSinceEpoch();
            float difSec = (curtimestamp - timestamp)/1000.f;
            m_chart->axes(Qt::Horizontal).first()->setRange(0, difSec);
            m_series->append(difSec,IncPID_data.cul_val);

        }
    }
}



//----------------------------------------------------------------------------
float Widget::CalcPID(IncPID *IncPID)
{
    //偏差
    IncPID->Ek = IncPID->set_val - IncPID->cul_val;

    //比例差值E[k]-E[k-1]
    IncPID->Kp_err = IncPID->Ek - IncPID->Ek_1;

    //积分差值E[k]
    IncPID->Ki_err = 0;
    //-- 抗积分饱和
    //---- 上过调
    if(IncPID->out_val > IncPID->val_max)
    {
        //防止进一步上过调
        if(IncPID->Ek <= 0)
        {
            IncPID->Ki_err = IncPID->Ek;
        }
    }
    //---- 下过调
    else if(IncPID->out_val < IncPID->val_min)
    {
        //防止进一步下过调
        if(IncPID->Ek >= 0)
        {
            IncPID->Ki_err = IncPID->Ek;
        }
    }
    //---- 未过调
    else
    {
        IncPID->Ki_err = IncPID->Ek;
    }

    //微分差值E[k]-2E[k-1]+E[k-2]
    IncPID->Kd_err = IncPID->Ek - IncPID->Ek_1 - IncPID->Ek_1 + IncPID->Ek_2;

    //本次输出
    IncPID->out_val  = IncPID->Kp * IncPID->Kp_err; //比例E[k]-E[k-1]
    IncPID->out_val += IncPID->Ki * IncPID->Ki_err; //积分E[k]
    IncPID->out_val += IncPID->Kd * IncPID->Kd_err; //微分E[k]-2E[k-1]+E[k-2]

    //输出限位
    if(IncPID->out_val < IncPID->val_min)
    {
        IncPID->out_val = IncPID->val_min;
    }
    else if(IncPID->out_val > IncPID->val_max)
    {
        IncPID->out_val = IncPID->val_max;
    }

    //更新E[k-2]
    IncPID->Ek_2 = IncPID->Ek_1; //E[k-2] = E[k-1]
    //更新E[k-1]
    IncPID->Ek_1 = IncPID->Ek; //E[k-1] = E[k]

    return IncPID->out_val;
}
