#include "wled.h"

#include "AutoHomeState.hpp"
#include "InitialState.hpp"
#include "IdleState.hpp"
#include "../gcodes.hpp"

AutoHomeState autoHomeState;

State* AutoHomeState::ProcessLine(const String& line) {
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

            } else if (line.equals(FPSTR(OkLine))) {
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
            }
            break;

        case HomingState::Homed:
            if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
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