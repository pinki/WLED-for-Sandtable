#include <pgmspace.h>

#include "IdleState.hpp"
#include "InitialState.hpp"
#include "RunState.hpp"
#include "PlaylistState.hpp"
#include "AlarmState.hpp"

using namespace SandtableUsermod;

IdleState SandtableUsermod::idleState;

const char IdleState::IndicatorLineStart[] PROGMEM = "<Idle|";

State* IdleState::ProcessLine(const String& line) {
    _lastProcessedLineAt = millis();

    if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
        DEBUG_PRINTF(NewStatePrintfDebugLine, alarmState.getName());

        alarmState.activate();
        return alarmState.ProcessLine(line);

    } else if (line.startsWith(FPSTR(RunState::IndicatorLineStart))) {
        DEBUG_PRINTF(NewStatePrintfDebugLine, runState.getName());

        runState.activate();
        return runState.ProcessLine(line);

    } else if (_configuration.isPlaylistActive) {
        DEBUG_PRINTF(NewStatePrintfDebugLine, playlistState.getName());

        playlistState.activate();
        return playlistState.ProcessLine(line);

    } else if (_motorPowerState != MotorPowerState::Off) {
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
                // Here is intentionally no break to return initial state in default case

            default:
                DEBUG_PRINTLN(F("ST> 🚨 UNKNOWN CONDITION! RETURNING TO INITIAL STATE! 🚨"));
                return &initialState;
        }
    }

    return this;
}