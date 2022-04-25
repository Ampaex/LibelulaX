#include "aircraft.hpp"
#include "esp_log.h"

Aircraft globalAircraft;

Aircraft::Aircraft() : controlMode(AUTOMATIC),
                       autopilot(this),
                       elevator(0.),
                       aileron(0.),
                       rudder(0.),
                       throttle(0.),
                       altitude(0.)

{
}

Aircraft::~Aircraft()
{
}

void Aircraft::setControlMode(ControlMode_t newControlMode)
{
    switch (newControlMode)
    {
    case MANUAL:
        if (autopilot.isEnabled())
            autopilot.disable();
        break;

    case SEMI_AUTOMATIC:
        ESP_LOGW(__func__, "Control mode %d not implemented yet", newControlMode);
        break;

    case AUTOMATIC:
        if (!autopilot.isEnabled())
            autopilot.enable();
        break;

    default:
        ESP_LOGW(__func__, "Control mode %d not implemented yet", newControlMode);
        break;
    }
}

void Aircraft::setThrottle(float pThrottle)
{
    autopilot.disableSpeed();
    throttle = pThrottle;
}

void Aircraft::printActuators()
{
    int size = (sizeof(actuators) / sizeof(float))-1;
    for (int i = 0; i < size; i++)
    {
        printf("%f,", *actuators[i]);
    }
    printf("%f\n", *actuators[size]);
    
}

float Aircraft::getAltitude()
{
    return altitude;
}