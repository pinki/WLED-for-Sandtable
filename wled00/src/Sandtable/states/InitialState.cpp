#include "wled.h"

#include "InitialState.hpp"
#include "IdleState.hpp"
#include "RunState.hpp"
#include "AutoHomeState.hpp"

InitialState initialState;

State* InitialState::ProcessLine(const String& line) {
    if (millis() - _activeSince > SANDTABLE_MAX_TIME_IN_INITIAL_STATE_BEFORE_REBOOT) {
        resetSandtable();
    }

    switch (_bootStage) {
        case 0:
            if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
                idleState.activate();
                return &idleState;

            } else if (line.startsWith(FPSTR(RunState::IndicatorLineStart))) {
                runState.activate();
                return &runState;

            } else if (line.startsWith(F("[MSG:INFO: FluidNC v"))) {
                _motorPowerState = MotorPowerState::Off;
                _bootStage++;
            }
            break;

        case 1:
            if (line.equals(F("[MSG:INFO: X Axis driver test passed]"))) {
                _bootStage++;
            }
            break;

        case 2:
            if (line.equals(F("[MSG:INFO: Y Axis driver test passed]"))) {
                _bootStage++;
            }
            break;

        case 3:
            if (line.equals(F("[MSG:INFO: '$H'|'$X' to unlock]"))) {
                DEBUG_PRINTF(NewStatePrintfDebugLine, autoHomeState.getName());

                autoHomeState.activate();
                return &autoHomeState;
            }
            break;
        
        default:
            break;
    }

    return this;
}

void InitialState::resetSandtable() {
    DEBUG_PRINTLN(F("ST> Initial state :: Resetting FluidNC...\n"));
    Serial2.println(FPSTR(GCode::RebootSystemCommand));

    _bootStage = 0;
    activate();
}

void InitialState::activate() {
    State::activate();

    _motorPowerState = MotorPowerState::Unknown;
}