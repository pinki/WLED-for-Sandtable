#include <pgmspace.h>

#include "RunState.hpp"
#include "IdleState.hpp"
#include "../gcodes.hpp"

RunState runState;

const char RunState::IndicatorLineStart[] PROGMEM = "<Run|";

State* RunState::ProcessLine(const String& line) {
    if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
        // TODO: Start next playlist entry

        DEBUG_PRINTF(NewStatePrintfDebugLine, idleState.getName());

        idleState.activate();
        return idleState.ProcessLine(line);
    }

    return this;
}