#pragma once

#include "wled.h"
#include "State.hpp"

typedef enum HomingState {
    Initial,
    HomeCommandSent,
    Homing,
    Homed
} HomingState;

class AutoHomeState : public State {
    private:
        HomingState _homingState = HomingState::Initial;

    public:
        State* ProcessLine(const String& line) override;

        const char* getName() override { return "Auto Home"; }
};

extern AutoHomeState autoHomeState;