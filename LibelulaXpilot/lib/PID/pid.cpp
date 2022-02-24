#pragma once

#include <iostream>
#include <cmath>
#include "pid.hpp"

using namespace std;

PID::PID(double max, double min, double Kp, double Ki, double Kd) : _max(max),
                                                                    _min(min),
                                                                    _Kp(Kp),
                                                                    _Ki(Ki),
                                                                    _Kd(Kd),
                                                                    _pre_error(0),
                                                                    _integral(0)
{
    _lt = esp_timer_get_time();
}

double PID::calculate(double setpoint, double pv)
{
    // Differential time since calculate was last called
    double dt = esp_timer_get_time() - _lt;

    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * dt;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _pre_error) / dt;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if (output > _max)
        output = _max;
    else if (output < _min)
        output = _min;

    // Save error to previous error
    _pre_error = error;

    return output;
}

void PID::setTuning(double Kp, double Ki, double Kd)
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
