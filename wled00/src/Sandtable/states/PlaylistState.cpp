#include <wled.h>

#include "PlaylistState.hpp"
#include "IdleState.hpp"
#include "AlarmState.hpp"
#include "../JsonKeys.hpp"

using namespace SandtableUsermod;

PlaylistState SandtableUsermod::playlistState;

State* PlaylistState::ProcessLine(const String& line) {
    _lastProcessedLineAt = millis();

    if (isLineOkForStateQueryCommand(line)) return this;

    if (line.startsWith(FPSTR(IdleState::IndicatorLineStart))) {
        auto configuration = getConfiguration();

        if (configuration->isPlaylistActive) {
            // Play next playlist item
            uint8_t index = getNextPlaylistItemIndex();
            play(_playlist.at(index));

        } else {
            DEBUG_PRINTF(NewStatePrintfDebugLine, idleState.getName());
            idleState.activate();

            return &idleState;
        }

    } else if (line.startsWith(FPSTR(AlarmState::IndicatorLineStart))) {
            DEBUG_PRINTF(NewStatePrintfDebugLine, alarmState.getName());
            alarmState.activate();

            return &alarmState;
    }

    return this;
}

void PlaylistState::activate() {
    State::activate();

    _motorPowerState = MotorPowerState::On;
}

void PlaylistState::writePlaylist(const JsonObject& parentObject) {
    JsonArray playlistArray = parentObject.createNestedArray(FPSTR(JsonKeys::configPlaylistKey));

    for (auto playlistItem : _playlist) {
        JsonObject playlistArrayItem = playlistArray.createNestedObject();
        playlistArrayItem[FPSTR(JsonKeys::configPlaylistItemFilepathKey)]   = playlistItem.filepath;
        playlistArrayItem[FPSTR(JsonKeys::configPlaylistItemPresetIdKey)]   = playlistItem.presetId;
        playlistArrayItem[FPSTR(JsonKeys::configPlaylistItemEraseAfterKey)] = playlistItem.eraseAfter;
    }
}

bool PlaylistState::updatePlaylist(const JsonArray& playlistArray) {
    if (playlistArray.isNull()) return false;

    std::vector<PlaylistEntry> readPlaylist;
    for (size_t i = 0; i < playlistArray.size(); i++) {
        JsonObject playlistItem = playlistArray.getElement(i);
        if (playlistItem.isNull()) continue;
        
        auto filepath   = playlistItem[FPSTR(JsonKeys::configPlaylistItemFilepathKey)];
        auto presetId   = playlistItem[FPSTR(JsonKeys::configPlaylistItemPresetIdKey)];
        auto eraseAfter = playlistItem[FPSTR(JsonKeys::configPlaylistItemEraseAfterKey)];

        if (filepath.isNull() || !filepath.is<String>()) continue;
        if (presetId.isNull() || !presetId.is<uint8_t>()) continue;
        if (eraseAfter.isNull() || !eraseAfter.is<bool>()) continue;

        PlaylistEntry entry;
        entry.filepath   = playlistItem[FPSTR(JsonKeys::configPlaylistItemFilepathKey)].as<String>();
        entry.presetId   = playlistItem[FPSTR(JsonKeys::configPlaylistItemPresetIdKey)].as<uint8_t>();
        entry.eraseAfter = playlistItem[FPSTR(JsonKeys::configPlaylistItemEraseAfterKey)].as<bool>();

        readPlaylist.push_back(entry);
    }


    DEBUG_PRINTLN(F("ST> Updating playlist"));
    DEBUG_PRINTLN(F("ST>  Before"));
    uint8_t index = 0;
    for (auto playlistItem : _playlist) {
        DEBUG_PRINTF("ST>  ✏️  Entry %hhu of %u: %s (%hhu)\n", ++index, _playlist.capacity(), playlistItem.filepath.c_str(), playlistItem.presetId);
    }

    _playlist = readPlaylist;
    _updates++;

    DEBUG_PRINTLN(F("ST>  After"));
    index = 0;
    for (auto playlistItem : readPlaylist) {
        DEBUG_PRINTF("ST>  ✏️  Entry %hhu of %u: %s (%hhu)\n", ++index, readPlaylist.capacity(), playlistItem.filepath.c_str(), playlistItem.presetId);
    }

    return true;
}

uint8_t PlaylistState::getNextPlaylistItemIndex() {
    static uint8_t nextIndex = 255;

    if (++nextIndex >= _playlist.size()) {
        nextIndex = 0;
    }

    return nextIndex;
}

void PlaylistState::play(const PlaylistEntry& entry) {
    DEBUG_PRINTF("ST> Playing '%s'...\n", entry.filepath.c_str());

    String filepath;
    String command;

    if (entry.filepath.startsWith(F("sdcard/"))) {
        filepath = entry.filepath.substring(6);
        DEBUG_PRINTF("ST> Filepath = %s\n", filepath.c_str());

        command = FPSTR(GCode::RunSDCardFileCommand);

    } else if (entry.filepath.startsWith(F("localfs/"))) {
        filepath = entry.filepath.substring(7);
        DEBUG_PRINTF("ST> Filepath = %s\n", filepath.c_str());

        command = FPSTR(GCode::RunLocalFSFileCommand);
    }

    applyPreset(entry.presetId, CALL_MODE_BUTTON_PRESET);

    if (filepath.length() > 0) {
        Serial2.printf("%s%s", command.c_str(), filepath.c_str());
    }
}