#pragma once

#include "State.hpp"

#define SANDTABLE_MAX_TIME_IN_INITIAL_STATE_BEFORE_REBOOT   15000

class InitialState : public State {
    private:
        uint8_t _bootStage = 0;

        void resetSandtable();

    public:
        State* ProcessLine(const String& line) override;

        const char* getName() override { return "Initial"; }

        void activate() override;
};

extern InitialState initialState;