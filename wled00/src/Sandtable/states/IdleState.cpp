#include <pgmspace.h>

#include "IdleState.hpp"
#include "InitialState.hpp"
#include "RunState.hpp"

IdleState idleState;

const char IdleState::IndicatorLineStart[] PROGMEM = "<Idle|";

State* IdleState::ProcessLine(const String& line) {
    if (line.startsWith(FPSTR(RunState::IndicatorLineStart))) {
        DEBUG_PRINTF(NewStatePrintfDebugLine, runState.getName());

        runState.activate();
        return runState.ProcessLine(line);
    }

    if (_motorPowerState == MotorPowerState::On) {
        switch (_disableMotorsCommandState) {
            case CommandState::NotSent:
                DEBUG_PRINTLN(F("ST> ❌ Disabling motors"));
                Serial2.println(FPSTR(GCode::DisableMotorsCommand));
                _disableMotorsCommandState = CommandState::Sent;
                break;

            case CommandState::Sent:
                if (line.equals(FPSTR(OkLine))) {
                    _disableMotorsCommandState = CommandState::NotSent;
                    _motorPowerState = MotorPowerState::Off;
                }
                break;

            case CommandState::Acknowledged:
                DEBUG_PRINTLN(F("ST> 🚨 Unexpected state: Motor on and off command acknowledged"));
                // Here is intentionally no break to return initial state

            default:
                DEBUG_PRINTLN(F("ST> 🚨 UNKNOWN CONDITION! RETURNING TO INITIAL STATE! 🚨"));
                return &initialState;
        }
    }

    return this;
}