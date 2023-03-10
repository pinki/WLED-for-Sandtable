#pragma once

#include "wled.h"
#include "../gcodes.hpp"

class State {
    protected:
        unsigned long _activeSince;

    public:
        State() {
            _activeSince = millis();
        }
        virtual ~State() { }

        virtual State& ProcessLine(const String& line) = 0;
        virtual String getName() = 0;

        void activate() {
            _activeSince = millis();
        }
};