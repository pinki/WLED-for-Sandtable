#include "wled.h"

#include "InitialState.hpp"
#include "AutoHomeState.hpp"
#include "IdleState.hpp"
#include "RunState.hpp"
#include "PlaylistState.hpp"
#include "AlarmState.hpp"

using namespace SandtableUsermod;

InitialState SandtableUsermod::initialState;

State* InitialState::ProcessLine(const String& line) {
    _lastProcessedLineAt = millis();

    if (_lastProcessedLineAt - _activeSince > _configuration.allowedBootUpTimeInSeconds * 1000) {
        resetSandtable();
    }

    switch (_bootStage) {
        case 0:
            if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
                DEBUG_PRINTF(NewStatePrintfDebugLine, idleState.getName());

                idleState.activate();
                return &idleState;

            } else if (line.startsWith(FPSTR(RunState::IndicatorLineStart))) {
                DEBUG_PRINTF(NewStatePrintfDebugLine, runState.getName());

                runState.activate();
                return &runState;

            } else if (line.startsWith(FPSTR(AlarmState::IndicatorLineStart))) {
                DEBUG_PRINTF(NewStatePrintfDebugLine, alarmState.getName());

                alarmState.activate();
                return &alarmState;

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
                if (_configuration.doAutoHome) {
                    DEBUG_PRINTF(NewStatePrintfDebugLine, autoHomeState.getName());

                    autoHomeState.activate();
                    return &autoHomeState;

                } else if (_configuration.isPlaylistActive) {
                    DEBUG_PRINTF(NewStatePrintfDebugLine, playlistState.getName());

                    playlistState.activate();
                    return &playlistState;

                } else {
                    DEBUG_PRINTF(NewStatePrintfDebugLine, idleState.getName());

                    idleState.activate();
                    return &idleState;
                }
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