#pragma once

#include <stddef.h>

class State {
    public:
        State() { }
        virtual ~State() { }

        virtual State& ProcessLine(const char* line, size_t length);
};