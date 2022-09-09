#include "autopilot.hpp"

#include "aircraft.hpp"
#include "esp_log.h"

static GPS_coordinate nextWaypoint;

struct update_args
{
    Autopilot *autopilot_arg;
    Aircraft *aircraft_arg;
};

void taskUpdateAutopilot(update_args *pArgs)
{
    Autopilot *_autopilot = pArgs->autopilot_arg;
    Aircraft *_aircraft = pArgs->aircraft_arg;
    float distance;
    int orbitCounter = 0;
    float minDistance = 1000; //TODO: Delete the minimum measurement
    float minCalc = 1000; //TODO: QUITAR
    while (true)
    {
        switch (_autopilot->getCurrentState())
        {
        case INIT:
            _aircraft->elevator = 0;
            _autopilot->disableAltitudeControl();
            _autopilot->disablePitch();
            vTaskDelay(pdMS_TO_TICKS(23000));

            if (_aircraft->getAltitude() < 3.)
                _autopilot->setCurrentState(TAKEOFF);
            else
                _autopilot->setCurrentState(ASCENSION);
            break;

        case TAKEOFF:
            if (_aircraft->getAltitude() > 1.)
            {
                _autopilot->setCurrentState(ASCENSION);
            }

            if (!_autopilot->gps.getHome())
            {
                _autopilot->gps.setHome(_autopilot->gps.getCurrentPosition());
                ESP_LOGI(__func__, "HOME: %f, %f\n", _autopilot->gps.getCurrentPosition().latitude, _autopilot->gps.getCurrentPosition().longitude);
            }

            _autopilot->clearPid();
            _autopilot->clearAltitudeControl();
            _autopilot->setAltitudeControl(350.);
            _autopilot->enableAltitudeControl();
            _autopilot->disablePathTracking();
            for (int i = 0; i < 100; i++) // Progressive acceleration
            {
                _aircraft->setThrottle(i * 0.01);
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            break;

        case ASCENSION:
            if (_aircraft->getAltitude() > _autopilot->getAltitudeTarget())
            {
                _autopilot->setCurrentState(STABLE);
            }
            _autopilot->setAltitudeControl(350.); // Target altitude
            _aircraft->setThrottle(0.9);
            break;

        case STABLE:
            if (_aircraft->altitude > 350. && _aircraft->altitude < 360.)
            {
                _autopilot->clearPathTracking();
                _autopilot->setCurrentState(NEW_CHECKPOINT);
            }
            _autopilot->setAltitudeControl(350.); // Target altitude
            _aircraft->setThrottle(0.9);
            break;

        case NEW_CHECKPOINT:
            _autopilot->enablePathTracking();
            _aircraft->setThrottle(0.9);
            ESP_LOGI(__func__,"####MINIMUM DISTANCE = %f\n", minDistance); //TODO: QUITAR
            if (_autopilot->gps.getPathSize() < 1)
            {
                nextWaypoint = _autopilot->gps.getHome();
                ESP_LOGI(__func__, "TO HOME\n");
                _autopilot->setCurrentState(TO_HOME);
            }
            else
            {
                nextWaypoint = _autopilot->gps.popCoord();
                _autopilot->setCurrentState(TO_CHECKPOINT);
                _autopilot->clearPathTracking();
                ESP_LOGI(__func__, "NEW WAYPOINT: %f, %f\n", nextWaypoint.latitude, nextWaypoint.longitude);
            }

            break;

        case TO_CHECKPOINT:
            distance = _autopilot->gps.distanceToCoord(nextWaypoint);
            minCalc =  _autopilot->gps.distanceToCoord({40.50240, -3.51499}); //TODO: QUITAR
            if(minCalc<minDistance)minDistance=minCalc; //TODO: QUITAR
            if (distance < 0.08) // If we're closer than 80m go to the next checkpoint
            {
                _autopilot->setCurrentState(NEW_CHECKPOINT);
            }
            break;

        case TO_HOME:
            distance = _autopilot->gps.distanceToCoord(nextWaypoint);
            if (distance < 0.3)
                _aircraft->setThrottle(0.2);
            if (distance < 0.1) // If we're closer than 80m go to the next checkpoint
            {
                _autopilot->setCurrentState(ORBIT_HOME);
            }
            break;

        case ORBIT_HOME:
            _aircraft->setThrottle(0.2);
            break;

        case PAUSE:
            vTaskDelay(pdMS_TO_TICKS(200));

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
                                                                           yaw(0.7, -0.7, 0.012, 0.0005, 0.00128, &(pAircraft->rudder), true, 360.), // 0.002
                                                                           roll(1, -1, 0.009, 0.0005, 0.009, &(pAircraft->aileron)),
                                                                           speed(1, -1, 0.003, 0.0005, 0.002, &(pAircraft->throttle)),
                                                                           altitudeControl(20, -20, 0.09, 0.0003, 0.06, &pitch)
{
    
    speed.disable();
    disablePathTracking();

    vector<GPS_coordinate> path = {{40.47644, -3.51327}, {40.50240, -3.51499}, {40.48792, -3.62282}};
    gps.appendPath(path); // TODO: Remove artificial coordinate

    //gps.appendCoordinate({40.47644, -3.51327});
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
    if(updateHandler == NULL){
        static update_args pArgs = {this, aircraft};
        xTaskCreate((TaskFunction_t)&taskUpdateAutopilot, "Autopilot_update", 4096, &pArgs, 5, &updateHandler);
    }
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

void Autopilot::reportRoll(float input)
{
    roll.calculate(input);
}

void Autopilot::reportSpeed(float input)
{
    speed.calculate(input);
}

void Autopilot::setAltitudeControl(float newAltitude)
{
    altitudeControl.setSetpoint(newAltitude);
}

void Autopilot::disableAltitudeControl()
{
    altitudeControl.disable();
}

void Autopilot::enableAltitudeControl()
{
    pitch.enable();
    altitudeControl.enable();
};

void Autopilot::reportCoordinate(GPS_coordinate coord)
{
    gps.setCurrentPosition(coord);
    aircraft->altitude = coord.altitude;
    altitudeControl.calculate(coord.altitude);
}

void Autopilot::reportCompass(float compass)
{
    gps.setCompass(compass);
    float compassAngle = gps.compassToCoord(nextWaypoint);
    // if(yaw.isEnabled())ESP_LOGI(__func__, "COMPASS2: %f\nCALCANGLE: %f \nRECTANGLE: %f\n", compass2, compassAngle, rectifiedAngle);
    //  printf("compass2: %f\n",compass2);
    yaw.setSetpoint(compassAngle);
    yaw.calculate(compass);
}