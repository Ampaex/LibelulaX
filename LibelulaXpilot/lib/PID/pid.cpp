#include <iostream>
#include <cmath>
#include "pid.hpp"

using namespace std;

PID::PID(int max, int min, float Kp, float Ki, float Kd) : _lt(esp_timer_get_time()),
                                                           _max(max),
                                                           _min(min),
                                                           _Kp(Kp),
                                                           _Ki(Ki),
                                                           _Kd(Kd),
                                                           _pre_error(0),
                                                           _integral(0)
{
}

float PID::calculate(float setpoint, float pv)
{
    // Differential time since calculate was last called
    int64_t dt = esp_timer_get_time() - _lt;

    // Calculate error
    float error = setpoint - pv;

    // Proportional term
    float Pout = _Kp * error;

    // Integral term
    _integral += error * dt;
    float Iout = _Ki * _integral;

    // Derivative term
    float derivative = (error - _pre_error) / dt;
    float Dout = _Kd * derivative;

    // Calculate total output
    float output = Pout + Iout + Dout;

    // Restrict to max/min
    if (output > _max)
        output = _max;
    else if (output < _min)
        output = _min;

    // Save error to previous error
    _pre_error = error;

    return output;
}

void PID::setTuning(float Kp, float Ki, float Kd)
{
    if (Kp < 0 || Ki < 0 || Kd < 0)
        return;

    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
}

PID::~PID()
{
}
