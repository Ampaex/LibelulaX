#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// TODO: Utilizar una cola para utilizar la media de error acumulado
class PID
{
private:
    int64_t _lt; // Microseconds when calculate was last called.
    int _max;
    int _min;
    float _Kp;
    float _Ki;
    float _Kd;
    float _pre_error;
    float _integral;

public:
    /*
    @param Kp Proportional gain
    @param Ki Integral gain
    @param Kd Derivative gain
    @param max Maximum value of manipulated variable
    @param min Minimum value of manipulated variable
    */
    PID(int max, int min, float Kp, float Kd, float Ki);

    // Returns the manipulated variable given a setpoint and current process value
    float calculate(float setpoint, float pv);

    void setTuning(float Kp, float Kd, float Ki);

    ~PID();
};
