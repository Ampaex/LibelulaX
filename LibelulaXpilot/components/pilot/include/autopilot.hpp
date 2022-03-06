#pragma once

enum ApStateMachine_t
{
    INIT = -1,
    TAKEOFF = 0,
    ASCENSION = 1,
    TO_CHECKPOINT = 2,
    NEW_CHECKPOINT = 3,
    TO_HOME = 4,
    ORBIT_HOME = 5
};

ApStateMachine_t actualState = INIT;

void startAutopilot(void);