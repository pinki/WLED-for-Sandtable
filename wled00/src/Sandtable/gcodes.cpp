#include <pgmspace.h>

#include "gcodes.hpp"

using namespace SandtableUsermod;

const char GCode::HomeCommand[]           PROGMEM = "$Home";
const char GCode::StateCommand[]          PROGMEM = "?";
const char GCode::RebootSystemCommand[]   PROGMEM = "$Bye";
const char GCode::DisableMotorsCommand[]  PROGMEM = "$MD";
const char GCode::RunSDCardFileCommand[]  PROGMEM = "$SD/Run=";
const char GCode::RunLocalFSFileCommand[] PROGMEM = "$LocalFS/Run=";