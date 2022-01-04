#include "widget.h"
#include "ui_widget.h"
int index;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    PID_init();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked(){
    ui->label_curspeed->setText(ui->lineEdit_startspeed->text());
    pid.curSpeed = ui->lineEdit_startspeed->text().toFloat();
    pid.SetSpeed = ui->lineEdit_targetspeed->text().toFloat();
    pid.Kp = ui->lineEdit_p->text().toFloat();
    pid.Ki = ui->lineEdit_i->text().toFloat();
    pid.Kd = ui->lineEdit_d->text().toFloat();
    m_timerID = startTimer(500);
}

void Widget::timerEvent(QTimerEvent *e){
    if(m_timerID == e->timerId()){
        float speed=PID_realize(pid.SetSpeed);
        ui->label_curspeed->setText(QString::number(speed));
        cout<<"速度为: "<<speed<<endl;
    }
}

void Widget::PID_init(){
    cout<<"PID_init begin \n";
    pid.curSpeed=0.0;
    pid.SetSpeed=50.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.voltage=0.0;
    pid.integral=0.0;
    pid.Kp=0.05;
    pid.Ki=0.05;
    pid.Kd=0.05;
    pid.umax=100;
    pid.umin=-100;
    cout<<"PID_init end \n";
    ui->label_curspeed->setText(QString::number(pid.curSpeed));
    ui->lineEdit_startspeed->setText(QString::number(pid.curSpeed));
    ui->lineEdit_targetspeed->setText(QString::number(pid.SetSpeed));
    ui->lineEdit_p->setText(QString::number(pid.Kp));
    ui->lineEdit_i->setText(QString::number(pid.Ki));
    ui->lineEdit_d->setText(QString::number(pid.Kd));
}


float Widget::PID_realize(float speed){
    pid.SetSpeed=speed;
    pid.err=pid.SetSpeed-pid.curSpeed;
    if(pid.curSpeed>pid.umax){
        //灰色底色表示抗积分饱和的实现
        if(abs(pid.err)>200){
        //蓝色标注为积分分离过程
            index=0;
        }
        else{
            index=1;
            if(pid.err<0){
                pid.integral+=pid.err;
            }
        }
    }
    else if(pid.curSpeed<pid.umin){
        if(abs(pid.err)>200){
        //积分分离过程
            index=0;
        }
        else{
            index=1;
            if(pid.err>0){
            pid.integral+=pid.err;
            }
        }
    }
    else{
        if(abs(pid.err)>200){
        //积分分离过程
            index=0;
        }
        else{
            index=1;
            pid.integral+=pid.err;
        }
    }
    pid.voltage=pid.Kp*pid.err+index*pid.Ki*pid.integral+pid.Kd*(pid.
    err-pid.err_last);
    pid.err_last=pid.err;
    pid.curSpeed=pid.voltage*1.0;
    return pid.curSpeed;
}
