#pragma once

#include "State.hpp"

class InitialState : public State {
    public:
        State& ProcessLine(const char* line, size_t length) {
            return *this;
        }
};

InitialState initialState;