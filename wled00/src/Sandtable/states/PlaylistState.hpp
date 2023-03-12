#pragma once

#include "State.hpp"

class PlaylistState : public State {
    public:
        State* ProcessLine(const String& line) override;

        const char* getName() override { return "Playlist"; }
};

extern PlaylistState playlistState;