#include <pgmspace.h>

#include "State.hpp"

using namespace SandtableUsermod;

MotorPowerState State::_motorPowerState = MotorPowerState::Unknown;
SandtableConfiguration State::_configuration = SandtableConfiguration();

unsigned long State::_lastProcessedLineAt = 0;
CommandState State::_queryStateCommandState = CommandState::NotSent;

const char State::OkLine[]                  PROGMEM = "ok";
const char State::MessageLineStart[]        PROGMEM = "[MSG: ";
const char State::NewStatePrintfDebugLine[]         = "ST> ℹ️ New state :: %s\n";

State::State() {
    _activeSince = millis();
}

void State::activate() {
    _activeSince = millis();
}

void State::queryStateIfNeeded() {
    auto elapsed = millis() - _lastProcessedLineAt;

    bool timeElapsedAndNoQuerySent = elapsed > _configuration.stateQueryInterval && _queryStateCommandState != CommandState::Sent;
    bool longTimeElapsedAndQueryNotAcknowledged = elapsed > _configuration.stateQueryInterval * 3 && _queryStateCommandState == CommandState::Sent;

    if (timeElapsedAndNoQuerySent || longTimeElapsedAndQueryNotAcknowledged) {
        DEBUG_PRINTLN(F("ST> Querying state"));
        Serial2.println(FPSTR(GCode::StateCommand));

        _queryStateCommandState = CommandState::Sent;
    }
}

SandtableConfiguration* const State::getConfiguration() {
    return &_configuration;
}

bool State::isLineOkForStateQueryCommand(const String& line) {
    if (_queryStateCommandState == CommandState::Sent && line.equals(FPSTR(OkLine))) {
        _queryStateCommandState = CommandState::Acknowledged;

        DEBUG_PRINTLN(F("ST> State query is acknowledged."));

        return true;
    }

    return false;
}