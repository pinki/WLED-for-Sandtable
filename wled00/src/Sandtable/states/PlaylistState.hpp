#pragma once

#include <vector>

#include "State.hpp"

namespace SandtableUsermod {
    typedef struct PlaylistEntry {
        String filename = "";
        uint8_t presetId = 0;
        bool eraseAfter = true;
    } PlaylistEntry;

    class PlaylistState : public State {
        private:
            std::vector<PlaylistEntry> _playlist;

        public:
            State* ProcessLine(const String& line) override;

            const char* getName() override { return "Playlist"; }
    };

    extern PlaylistState playlistState;
}