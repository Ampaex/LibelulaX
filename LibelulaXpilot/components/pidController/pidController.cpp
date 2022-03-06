#include "pidController.hpp"

#include "esp_log.h"

/*All PID instances*/

static PID testPid(1, -1, 0.002, 0.001, 0.002, TEST); // PID control for testing purposes
static PID elevatorPid(1, -1, -0.003, -0.0005, -0.002, ELEVATOR);
static PID speedPid(1, 0, 0.4, 0.1, 0.5, SPEED);


void sendToPID(PIDdata pidData)
{
    // Check if the datatype is not NONE
    if (pidData._datatype)
        getPIDwithType(pidData._datatype)->calculate(pidData._value);
}

PIDdata getFromPID(pidType_t pidType)
{
    return getPIDwithType(pidType)->getPIDdata();
}

void setPIDsetpoint(PIDdata setpoint)
{
    getPIDwithType(setpoint._datatype)->setSetpoint(setpoint._value);
}

PID *getPIDwithType(pidType_t pidType)
{
    switch (pidType)
    {
    case TEST:
        return &testPid;
        break;
    
    case ELEVATOR:
        return &elevatorPid;
        break;

    case SPEED:
        return &speedPid;
        break;

    default:
        return NULL;
    }
}