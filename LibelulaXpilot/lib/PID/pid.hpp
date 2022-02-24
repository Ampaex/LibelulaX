#pragma once

#include <freertos/FreeRTOS.h>

class PID
{
public:
    /*
    @param Kp Proportional gain
    @param Ki Integral gain
    @param Kd Derivative gain
    @param max Maximum value of manipulated variable
    @param min Minimum value of manipulated variable
    */
    PID(double max, double min, double Kp, double Kd, double Ki);

    // Returns the manipulated variable given a setpoint and current process value
    double calculate(double setpoint, double pv);

    void setTuning(double Kp, double Kd, double Ki);

    ~PID();

private:
    int64_t _lt; // Microseconds when calculate was last called.
    double _max;
    double _min;
    double _Kp;
    double _Kd;
    double _Ki;
    double _pre_error;
    double _integral;
};
