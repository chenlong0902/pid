#ifndef DEF_H
#define DEF_H


//typedef struct _pid{
//float SetSpeed;//定义设定值
//float curSpeed;//定义实际值
//float err;//定义偏差值
//float err_last;//定义上一个偏差值
//float Kp,Ki,Kd;//定义比例、积分、微分系数
//float voltage;//定义电压值(控制执行器的变量)
//float integral;//定义积分值
//float umax;//执行机构可执行的最大值
//float umin;//执行机构可执行的最小值
//}PID;

//增量式PID
struct IncPID
{
    float set_val; //设定值
    float cul_val; //当前值
    float out_val; //输出值

    float val_min; //最小值
    float val_max; //最大值

    float Kp; //比例系数Proportional
    float Ki; //积分系数Integral
    float Kd; //微分系数Derivative

    float Kp_err; //比例项差值
    float Ki_err; //积分项差值
    float Kd_err; //微分项差值

    float Ek;   //当前差值E[k]
    float Ek_1; //上次差值E[k-1]
    float Ek_2; //上上次差值E[k-2]
};
#endif // DEF_H
