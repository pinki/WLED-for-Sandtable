#pragma once

#include "State.hpp"

class IdleState : public State {
    public:
        State& ProcessLine(const String& line) override {
            return *this;
        }

        String getName() override { return "Idle"; }
};

extern IdleState idleState;