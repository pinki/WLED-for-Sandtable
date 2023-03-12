#include <pgmspace.h>

#include "gcodes.hpp"

const char GCode::HomeCommand[]          PROGMEM = "$Home";
const char GCode::StateCommand[]         PROGMEM = "$State";
const char GCode::RebootSystemCommand[]  PROGMEM = "$Bye";
const char GCode::DisableMotorsCommand[] PROGMEM = "$MD";