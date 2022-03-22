#pragma once

#include <freertos/FreeRTOS.h>

// TODO: Utilizar una cola para utilizar la media de error acumulado

class PID
{
private:
    int64_t _lt; // Microseconds since calculate was last called.
    float _max;
    float _min;
    float _Kp, _Ki, _Kd;
    float _pre_error;
    float _integral;

    float* _outputVar;
    PID* _outputPid;

    float _setpoint;

    bool _enabled = true;
    bool _secondaryPid = false; //Tells if this PID is controlling another.

    bool _circular = false;     //Whether the input is circular or not
    float _minC = 0.;    //Minimum input value
    float _maxC = 0.;    //Maximum input value
    float _midpoint;
    float _range;


public:
    /* Constructor for primary PID, which controls an actuator variable.
    @param Kp Proportional gain
    @param Ki Integral gain
    @param Kd Derivative gain
    @param max Maximum value of manipulated variable
    @param min Minimum value of manipulated variable
    @param outputVar Variable to store the resulting value.
    @param circularInput If the input variable is circular. (The ends are connected)
    @param maxC Maximum value for input.
    @param minC Minimum value for input.
    */
    PID(float max, float min, float Kp, float Ki, float Kd, float* outputVar, bool circularInput = false, float maxC=0., float minC=0.);

    /* Constructor for secondary PID, which controls another.
    @param Kp Proportional gain
    @param Ki Integral gain
    @param Kd Derivative gain
    @param max Maximum value of manipulated variable
    @param min Minimum value of manipulated variable
    @param outputVar Variable to store the resulting value.
    */
    PID(float max, float min, float Kp, float Ki, float Kd, PID* outputPid);


    // Receive new data and processses it to a new value trying to achieve the setpoint.
    void calculate(float pv);

    void setTuning(float Kp, float Ki, float Kd);
    void setP(float Kp) { _Kp = Kp; };
    void setI(float Ki) { _Ki = Ki; };
    void setD(float Kd) { _Kd = Kd; };

    void setRange(float max, float min);

    void setSetpoint(float newSetPoint);
    float getSetpoint(){return _setpoint;};

    // Get the actual value of PID
    float getValue() { return *_outputVar; };

    // Clear all accumulated values.
    void clear();

    bool isEnabled();
    //Enable calculate function and output
    void enable();
    //Disable calculate function and output
    void disable();

    void print();

    ~PID();
};