#pragma once

#include "wled.h"
#include "../gcodes.hpp"

typedef enum MotorPowerState {
    Unknown,
    On,
    Off
} MotorPowerState;

typedef enum CommandState {
    NotSent,
    Sent,
    Acknowledged
} CommandState;

class State {
    protected:
        unsigned long _activeSince;
        static MotorPowerState _motorPowerState;

        static const char OkLine[];
        static const char NewStatePrintfDebugLine[];

    public:
        State();
        virtual ~State() { }

        virtual State* ProcessLine(const String& line) = 0;
        virtual const char* getName() = 0;

        virtual void activate();
};