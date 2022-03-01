#include <iostream>
#include <cmath>
#include "pid.hpp"

using namespace std;

PID::PID(int max, int min, float Kp, float Ki, float Kd, pidType_t type) : _lt(esp_timer_get_time()),
                                                                           _max(max),
                                                                           _min(min),
                                                                           _Kp(Kp),
                                                                           _Ki(Ki),
                                                                           _Kd(Kd),
                                                                           _pre_error(0),
                                                                           _integral(0),
                                                                           _value(0.),
                                                                           _type(type),
                                                                           _setpoint(0.)
{
}

void PID::calculate(float pv)
{
    int64_t nt = esp_timer_get_time(); // Now time

    float dt = (nt - _lt) / 1000000.; // Differential time since calculate was last called

    _lt = nt;

    // Calculate error
    float error = _setpoint - pv;

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

    _value = output;
}

void PID::setTuning(float Kp, float Ki, float Kd)
{
    if (Kp < 0 || Ki < 0 || Kd < 0)
        return;

    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
}

void PID::setRange(int max, int min)
{
    _max = max;
    _min = min;
}

PIDdata PID::getPIDdata()
{
    PIDdata ret = {_type, _value};
    return ret;
}

void PID::print()
{
    printf("PIDclass\n _lt: %ld\n _max: %d\n _min: %d\n _Kp: %f\n _Ki: %f\n _Kd: %f\n _pre_error: %f\n _integral: %f\n _value: %f\n _type: %d\n _setpoint: %f\n", (long int)_lt, _max, _min, _Kp, _Ki, _Kd, _pre_error, _integral, _value, (int)_type, _setpoint);
}

PID::~PID()
{
}
