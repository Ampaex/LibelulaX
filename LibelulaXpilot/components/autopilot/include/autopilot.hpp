#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pid.hpp"
#include "gps.hpp"

// Autopilot state machine
enum ApStateMachine_t
{
    INIT = -1,
    PAUSE = 0,
    TAKEOFF = 1,
    ASCENSION = 2,
    STABLE = 3,
    NEW_CHECKPOINT = 4,
    TO_CHECKPOINT = 5,
    TO_HOME = 6,
    ORBIT_HOME = 7
};

class Aircraft;

class Autopilot
{
private:
    TaskHandle_t updateHandler;
    Aircraft *aircraft;
    bool enabled;

    ApStateMachine_t currentState;

    // Orientation
    PID pitch, yaw, roll;
    PID speed;

    // Grouping array
    PID *pidControllers[4] = {&pitch, &yaw, &roll, &speed};

    //Secondary PID for pitch
    PID altitudeControl;

public:

    GPS gps;

    bool isEnabled();
    void enable();
    void disable();

    // @param initialState Sets the initial state.
    Autopilot(Aircraft *pAircraft, ApStateMachine_t initialState = INIT);
    ~Autopilot();

    // Input the current pitch
    void reportPitch(float input);

    // Input the current roll
    void reportRoll(float input);

    // Input the current speed and enables the speed control
    void reportSpeed(float input);

    // Input the current coordinate
    void reportCoordinate(GPS_coordinate coord);

    void reportCompass(float compass);

    /**
     * @brief Set the pid values for getting an orientation.
     *
     * @param pPitch Pitch in degrees.
     * @param pYaw   Yaw in degrees.
     * @param pRoll  Roll in degrees.
     */
    void setOrientation(float pPitch, float pYaw, float pRoll);

    /**
     * @brief Set the speed.
     * Disable throttle control
     *
     * @param pSpeed  Speed in km/h.
     */
    void setSpeed(float pSpeed);

    void disableSpeed(){speed.disable();};

    ApStateMachine_t getCurrentState(){return currentState;};
    void setCurrentState(ApStateMachine_t newState){currentState = newState;};

    //Drive to the new altitude.
    void setAltitudeControl(float newAltitude);
    void clearAltitudeControl(){altitudeControl.clear();};
    void disableAltitudeControl();
    void enableAltitudeControl();

    void disablePitch(){pitch.disable();};

    //@return The target control altitude.
    float getAltitudeTarget(){return altitudeControl.getSetpoint();};

    void disablePathTracking(){yaw.disable();};

    void enablePathTracking(){yaw.enable();};

    void clearPathTracking(){yaw.clear();};

    void printYaw(){yaw.print();};

    // Clear every pid temporal variable
    void clearPid();
};