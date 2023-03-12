#pragma once

#include "State.hpp"

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