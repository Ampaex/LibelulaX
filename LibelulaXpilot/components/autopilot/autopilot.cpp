#include "autopilot.hpp"

#include "aircraft.hpp"
#include "esp_log.h"

struct update_args
{
    Autopilot *autopilot_arg;
    Aircraft *aircraft_arg;
};
void taskUpdateAutopilot(update_args *pArgs)
{
    Autopilot *_autopilot = pArgs->autopilot_arg;
    Aircraft *_aircraft = pArgs->aircraft_arg;
    while (true)
    {
        switch (_autopilot->getCurrentState())
        {
        case INIT:
            vTaskDelay(pdMS_TO_TICKS(14000));
            if (_aircraft->getAltitude() < 6.)
                _autopilot->setCurrentState(TAKEOFF);
            else
                _autopilot->setCurrentState(ASCENSION);
            break;

        case TAKEOFF:
            if (_aircraft->getAltitude() > 6.)
                _autopilot->setCurrentState(ASCENSION);

            _autopilot->clearPid();
            _autopilot->clearAltitudeControl();
            _autopilot->setAltitudeControl(350.);
            for(int i = 0; i<100; i++)   //Progressive acceleration
            {
                _aircraft->setThrottle(i*0.01);
                vTaskDelay(pdMS_TO_TICKS(50));
            }
            break;

        case ASCENSION:
            if (_aircraft->getAltitude() > _autopilot->getAltitudeTarget())
                _autopilot->setCurrentState(STABLE);
            _autopilot->setAltitudeControl(350.); //Target altitude
            _aircraft->setThrottle(0.8);
            break;

        case STABLE:
            _autopilot->setAltitudeControl(350.); //Target altitude
            _aircraft->setThrottle(0.7);
            break;

        case TO_CHECKPOINT:
            break;

        case NEW_CHECKPOINT:
            break;

        case TO_HOME:
            break;

        case ORBIT_HOME:
            break;

        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(CONFIG_AP_UPDATE_PERIOD));
    }
    vTaskDelete(NULL);
}

Autopilot::Autopilot(Aircraft *pAircraft, ApStateMachine_t initialState) : aircraft(pAircraft),
                                                                           enabled(true),
                                                                           currentState(initialState),
                                                                           pitch(1, -1, -0.009, -0.0005, -0.009, &(pAircraft->elevator)),
                                                                           yaw(1, -1, 0.003, 0.0005, 0.002, &(pAircraft->rudder)),
                                                                           roll(1, -1, 0.009, 0.0005, 0.009, &(pAircraft->aileron)),
                                                                           speed(1, -1, 0.003, 0.0005, 0.002, &(pAircraft->throttle)),
                                                                           altitudeControl(20, -20, 0.1, 0.001, 0.12, &pitch)
{
    static update_args pArgs = {this, pAircraft};
    speed.disable();
    xTaskCreate((TaskFunction_t)&taskUpdateAutopilot, "Autopilot_update", 2048, &pArgs, 5, &updateHandler);
}

Autopilot::~Autopilot()
{
    vTaskDelete(updateHandler);
}

bool Autopilot::isEnabled()
{
    return enabled;
}

void Autopilot::enable()
{
    enabled = true;
    currentState = INIT;
    vTaskResume(updateHandler);
}

void Autopilot::disable()
{
    enabled = false;
    currentState = PAUSE;
    vTaskSuspend(updateHandler);
}

void Autopilot::setOrientation(float pPitch, float pYaw, float pRoll)
{
    altitudeControl.disable();
    pitch.setSetpoint(pPitch);
    yaw.setSetpoint(pYaw);
    roll.setSetpoint(pRoll);
}

void Autopilot::setSpeed(float pSpeed)
{
    speed.enable();
    speed.setSetpoint(pSpeed);
}

void Autopilot::clearPid()
{
    pitch.clear();
    yaw.clear();
    roll.clear();
    speed.clear();
}

void Autopilot::reportPitch(float input)
{
    pitch.calculate(input);
}

void Autopilot::reportYaw(float input)
{
    yaw.calculate(input);
}

void Autopilot::reportRoll(float input)
{
    roll.calculate(input);
}

void Autopilot::reportSpeed(float input)
{
    speed.calculate(input);
}

void Autopilot::reportAltitude(float input)
{
    altitudeControl.calculate(input);
}

void Autopilot::setAltitudeControl(float newAltitude)
{
    altitudeControl.enable();
    altitudeControl.setSetpoint(newAltitude);
} 