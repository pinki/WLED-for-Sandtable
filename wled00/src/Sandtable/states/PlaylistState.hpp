#pragma once

#include <vector>

#include "State.hpp"

namespace SandtableUsermod {
    struct PlaylistEntry {
        String filepath = "";
        uint8_t presetId = 0;
        bool eraseAfter = true;
    };

    class PlaylistState : public State {
        private:
            std::vector<PlaylistEntry> _playlist;
            uint8_t _updates = 0;

            uint8_t getNextPlaylistItemIndex();
            void play(const PlaylistEntry& entry);

        public:
            PlaylistState() {
                PlaylistEntry entry;
                entry.filepath = "sdcard/Hexagons.gcode";
                entry.presetId = 1;
                entry.eraseAfter = true;

                _playlist.push_back(entry);
            }

            State* ProcessLine(const String& line) override;

            const char* getName() override { return "Playlist"; }

            uint8_t getUpdateCount() { return _updates; }

            const std::vector<PlaylistEntry> getPlaylist() { return _playlist; }
            void writePlaylist(const JsonObject& parentObject);
            void updatePlaylist(const JsonArray& playlistArray);
    };

    extern PlaylistState playlistState;
}