#pragma once

#include "wled.h"
#include "State.hpp"

namespace SandtableUsermod {
    class AlarmState : public State {
        public:
            static const char IndicatorLineStart[];

            State* ProcessLine(const String& line) override;

            const char* getName() override { return "Alarm"; }
    };

    extern AlarmState alarmState;
}