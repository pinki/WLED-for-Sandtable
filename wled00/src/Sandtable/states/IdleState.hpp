#pragma once

#include "State.hpp"

namespace SandtableUsermod {
    class IdleState : public State {
        private:
            CommandState _disableMotorsCommandState = CommandState::NotSent;

        public:
            static const char IndicatorLineStart[];

            State* ProcessLine(const String& line) override;

            const char* getName() override { return "Idle"; }
    };

    extern IdleState idleState;
}