#include <iostream>
#include <cmath>
#include "pid.hpp"

using namespace std;

PID::PID(float max, float min, float Kp, float Ki, float Kd, float *outputVar, bool circularInput, float maxC, float minC) : _lt(esp_timer_get_time()),
                                                                                                                             _max(max),
                                                                                                                             _min(min),
                                                                                                                             _Kp(Kp),
                                                                                                                             _Ki(Ki),
                                                                                                                             _Kd(Kd),
                                                                                                                             _pre_error(0),
                                                                                                                             _integral(0),
                                                                                                                             _outputVar(outputVar),
                                                                                                                             _setpoint(0.),
                                                                                                                             _circular(circularInput),
                                                                                                                             _minC(minC),
                                                                                                                             _maxC(maxC)
{
    _range = maxC - minC;
    _midpoint = (_range/2.) + minC;
}

PID::PID(float max, float min, float Kp, float Ki, float Kd, PID *outputPid) : _lt(esp_timer_get_time()),
                                                                               _max(max),
                                                                               _min(min),
                                                                               _Kp(Kp),
                                                                               _Ki(Ki),
                                                                               _Kd(Kd),
                                                                               _pre_error(0),
                                                                               _integral(0),
                                                                               _outputPid(outputPid),
                                                                               _setpoint(0.),
                                                                               _secondaryPid(true)
{
}

void PID::calculate(float pv)
{
    int64_t nt = esp_timer_get_time(); // Now time
    double dt = (nt - _lt) / 500000.;  // Differential time since calculate was last called in a period of a half second
    _lt = nt;
    if (_enabled)
    {
        float error;
        // Calculate error
        if (_circular)
        {
            error = _setpoint - pv;
            if (fabs(error) > _midpoint)
                error = (error > 0.) ? error - _range : _range + error;
        }
        else
        {
            error = _setpoint - pv;
        }

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

        if (_secondaryPid)
        {
            _outputPid->setSetpoint(output);
        }
        else
        {
            *_outputVar = output;
        }
    }
}

void PID::setTuning(float Kp, float Ki, float Kd)
{
    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
}

void PID::setRange(float max, float min)
{
    _max = max;
    _min = min;
}

void PID::setSetpoint(float newSetPoint)
{
    _setpoint = newSetPoint;
}

void PID::clear()
{
    _pre_error = 0.;
    _integral = 0.;
}

void PID::print()
{
    printf("PIDclass\n _lt: %ld\n _max: %f\n _min: %f\n _Kp: %f\n _Ki: %f\n _Kd: %f\n _pre_error: %f\n _integral: %f\n _outputVar: %f\n _setpoint: %f\n _enabled: %d\n", (long int)_lt, _max, _min, _Kp, _Ki, _Kd, _pre_error, _integral, *_outputVar, _setpoint, _enabled);
}

PID::~PID()
{
}

bool PID::isEnabled()
{
    return _enabled;
}

void PID::enable()
{
    _enabled = true;
}

void PID::disable()
{
    _enabled = false;
}
