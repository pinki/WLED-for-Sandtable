#include "wled.h"

#include "AutoHomeState.hpp"
#include "InitialState.hpp"
#include "IdleState.hpp"
#include "../gcodes.hpp"

using namespace SandtableUsermod;

AutoHomeState SandtableUsermod::autoHomeState;

State* AutoHomeState::ProcessLine(const String& line) {
    _lastProcessedLineAt = millis();
    
    if (isLineOkForStateQueryCommand(line)) return this;
    
    switch (_homingState) {
        case HomingState::Initial:
            DEBUG_PRINTLN(F("ST> Sending home command"));
            Serial2.println(FPSTR(GCode::HomeCommand));

            _homingState = HomingState::HomeCommandSent;
            break;

        case HomingState::HomeCommandSent:
            if (line.startsWith(F("<Home|"))) {
                _homingState = HomingState::Homing;
                _motorPowerState = MotorPowerState::On;

            } else if (line.equals(FPSTR(OkLine)) || line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
                _homingState = HomingState::Homed;
                _motorPowerState = MotorPowerState::On;

                DEBUG_PRINTF(NewStatePrintfDebugLine, idleState.getName());

                idleState.activate();
                return &idleState;

            }
            break;

        case HomingState::Homing:
            if (line.equals(FPSTR(OkLine))) {
                _homingState = HomingState::Homed;
                break;
            }
            // No break here, because it could be, that it's already homed.

        case HomingState::Homed:
            if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
                _homingState = HomingState::Homed; // See previous state

                DEBUG_PRINTF(NewStatePrintfDebugLine, idleState.getName());

                idleState.activate();
                return idleState.ProcessLine(line);
            }
            break;

        default:
            DEBUG_PRINTLN(F("ST> 🚨 UNKNOWN CONDITION! RETURNING TO INITIAL STATE! 🚨"));
            return &initialState;
    }

    return this;
}

void AutoHomeState::activate() {
    State::activate();

    _motorPowerState = MotorPowerState::On;

    DEBUG_PRINTLN(F("ST> Sending home command"));
    Serial2.println(FPSTR(GCode::HomeCommand));

    _homingState = HomingState::HomeCommandSent;
}