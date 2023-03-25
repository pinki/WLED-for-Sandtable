#include <pgmspace.h>

#include "AlarmState.hpp"
#include "IdleState.hpp"

using namespace SandtableUsermod;

AlarmState SandtableUsermod::alarmState;

const char AlarmState::IndicatorLineStart[] PROGMEM = "<Alarm|";

State* AlarmState::ProcessLine(const String& line) {
    _lastProcessedLineAt = millis();

    if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
        DEBUG_PRINTF(NewStatePrintfDebugLine, idleState.getName());

        idleState.activate();
        return &idleState;
    }

    return this;
}