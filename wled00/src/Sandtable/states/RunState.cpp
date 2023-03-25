#include <pgmspace.h>

#include "RunState.hpp"
#include "IdleState.hpp"
#include "PlaylistState.hpp"
#include "../gcodes.hpp"

using namespace SandtableUsermod;

RunState SandtableUsermod::runState;

const char RunState::IndicatorLineStart[] PROGMEM = "<Run|";

State* RunState::ProcessLine(const String& line) {
    _lastProcessedLineAt = millis();

    if (isLineOkForStateQueryCommand(line)) return this;

    if (_configuration.isPlaylistActive) {
        DEBUG_PRINTF(NewStatePrintfDebugLine, playlistState.getName());

        playlistState.activate();
        return playlistState.ProcessLine(line);

    } else if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
        DEBUG_PRINTF(NewStatePrintfDebugLine, idleState.getName());

        idleState.activate();
        return idleState.ProcessLine(line);
    }

    return this;
}

void RunState::activate() {
    State::activate();

    _motorPowerState = MotorPowerState::On;
}