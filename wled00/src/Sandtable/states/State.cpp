#include <pgmspace.h>

#include "State.hpp"

MotorPowerState State::_motorPowerState = MotorPowerState::Unknown;
SandtableConfiguration State::_configuration = SandtableConfiguration();

const char State::OkLine[]                  PROGMEM = "ok";
const char State::NewStatePrintfDebugLine[]         = "ST> ℹ️ New state :: %s\n";

State::State() {
    _activeSince = millis();
}

void State::activate() {
    _activeSince = millis();
}

SandtableConfiguration* const State::getConfiguration() {
    return &_configuration;
}