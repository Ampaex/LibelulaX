#include "pid.hpp"

// Sends PIDdata for PID processing
void sendToPID(PIDdata pidData);

// Get from the PID controller the type of data requested
PIDdata getFromPID(pidType_t pidType);

//Set a new setpoint for the pid specific controller
void setPIDsetpoint(PIDdata setpoint);

PID* getPIDwithType(pidType_t pidType);