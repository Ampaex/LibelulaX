#include "pidController.hpp"

#include "esp_log.h"

/*All PID instances*/

static PID testPid(130, -130, 1., 0.2, 1., TEST);    //PID control for testing purposes


void sendToPID(PIDdata pidData)
{
    // Check if the datatype is not NONE
    if(pidData._datatype)getPIDwithType(pidData._datatype)->calculate(pidData._value);
}

PIDdata getFromPID(pidType_t pidType)
{
    return getPIDwithType(pidType)->getPIDdata();
}

void setPIDsetpoint(PIDdata setpoint)
{
    getPIDwithType(setpoint._datatype)->setSetpoint(setpoint._value);
}

PID* getPIDwithType(pidType_t pidType)
{
    switch (pidType)
    {
    case TEST:
        return &testPid;
    
    default:
        return NULL;
    }
}