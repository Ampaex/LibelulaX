#pragma once

#include <freertos/FreeRTOS.h>

// Different types of input for PID control
enum pidType_t
{
    NONE = -1,
    TEST = 1,
    ELEVATOR = 2,
    SPEED = 3

};

// Data struct for communication
struct PIDdata
{
    pidType_t _datatype;
    float _value;
    PIDdata(pidType_t datatype, float value):_datatype(datatype),_value(value){};
};

// TODO: Utilizar una cola para utilizar la media de error acumulado

class PID
{
private:
    int64_t _lt; // Microseconds since calculate was last called.
    int _max;
    int _min;
    float _Kp;
    float _Ki;
    float _Kd;
    float _pre_error;
    float _integral;

    float _value;   // Actual value
    pidType_t _type;// Which type of data is controlling

    float _setpoint;

public:
    /*
    @param Kp Proportional gain
    @param Ki Integral gain
    @param Kd Derivative gain
    @param max Maximum value of manipulated variable
    @param min Minimum value of manipulated variable
    */
    PID(int max, int min, float Kp, float Ki, float Kd, pidType_t type);
    
    // Receive new data and processses it to a new value trying to achieve the setpoint.
    void calculate(float pv);

    void setTuning(float Kp, float Ki, float Kd);

    void setP(float Kp){_Kp = Kp;};
    void setI(float Ki){_Ki = Ki;};
    void setD(float Kd){_Kd = Kd;};

    void setRange(int max, int min);

    // Get the actual value of PID
    float getValue(){return _value;};

    void setSetpoint(float newSetPoint){_setpoint = newSetPoint;};

    void print();

    //@return Actual value in PIDdata format
    PIDdata getPIDdata();

    ~PID();

};