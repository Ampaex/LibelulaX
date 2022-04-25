#pragma once

#include "autopilot.hpp"

// Control modes
enum ControlMode_t
{
    MANUAL = 0,
    SEMI_AUTOMATIC = 1,
    AUTOMATIC = 2
};

class Aircraft
{
private:
    ControlMode_t controlMode;

public:

    Autopilot autopilot;

    // Actuators
    float elevator, aileron, rudder, throttle;

    // Altitude in ft
    float altitude;

    // Gouping array
    float *actuators[4] = {&elevator, &aileron, &rudder, &throttle};

    Aircraft();
    ~Aircraft();

    /**
     * @brief Set the control mode
     * @param newControlMode New type of control
     */
    void setControlMode(ControlMode_t newControlMode);

    // Print actuators separated by commas.
    void printActuators();

    /**
     * @brief Set throttle revolutions.
     * Disable speed control from autopilot.
     *
     * @param pThrottle Throttle, from 0 to 1.
     */
    void setThrottle(float pThrottle);


    //Return the altitude
    float getAltitude();
};

extern Aircraft globalAircraft;