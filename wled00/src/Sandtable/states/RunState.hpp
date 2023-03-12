#pragma once

#include "State.hpp"

class RunState : public State {
    public:
        static const char IndicatorLineStart[];

        State* ProcessLine(const String& line) override;

        const char* getName() override { return "Run"; }

        void activate() override;
};

extern RunState runState;