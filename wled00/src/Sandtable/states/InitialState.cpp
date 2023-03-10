#include "wled.h"

#include "InitialState.hpp"
#include "IdleState.hpp"

InitialState initialState;

State& InitialState::ProcessLine(const String& line) {
    if (millis() - _activeSince > SANDTABLE_MAX_TIME_IN_INITIAL_STATE_BEFORE_REBOOT) {
        // Reset FluidNC
        DEBUG_PRINTLN(F("ST> Initial state :: Resetting FluidNC...\n"));
        Serial2.println(GCode::RebootSystemCommand);

        _bootStage = 0;
        activate();
    }

    switch (_bootStage) {
        case 0:
            if (line.startsWith(F("[MSG:INFO: FluidNC v"))) _bootStage++;
            break;

        case 1:
            if (line.equals(F("[MSG:INFO: X Axis driver test passed]"))); _bootStage++;
            break;

        case 2:
            if (line.equals(F("[MSG:INFO: Y Axis driver test passed]"))); _bootStage++;
            break;

        case 3:
            if (line.equals(F("[MSG:INFO: '$H'|'$X' to unlock]"))) {
                DEBUG_PRINTF("ST> New state :: %s\n", idleState.getName().c_str());

                idleState.activate();
                return idleState;
            }
            break;
        
        default:
            break;
    }

    return *this;
}