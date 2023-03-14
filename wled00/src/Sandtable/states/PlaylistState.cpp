#include "PlaylistState.hpp"

using namespace SandtableUsermod;

PlaylistState SandtableUsermod::playlistState;

State* PlaylistState::ProcessLine(const String& line) {
    return this;
}