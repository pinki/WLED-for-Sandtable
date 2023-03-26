#include "Sandtable.hpp"
#include "JsonKeys.hpp"
#include "states/PlaylistState.hpp"

using namespace SandtableUsermod;

void Sandtable::loop() {
    if (Serial2.available()) {
        String line = Serial2.readStringUntil('\r');
        {
            // Skip \n
            static char notInUse[1];
            Serial2.readBytes(notInUse, 1);
        }

        if (line.length()) {
            DEBUG_PRINTF("ST> %s state is processing line: %s\n", _currentState->getName(), line.c_str());

            _currentState = _currentState->ProcessLine(line);
        }

    } else if (_writePlaylistToDebugOutput) {
        _writePlaylistToDebugOutput = false;

        uint8_t index = 0;
        auto playlist = playlistState.getPlaylist();
        DEBUG_PRINTF("ST> Playlist (%hhu Updates):\n", playlistState.getUpdateCount());
        for (auto playlistItem : playlist) {
            DEBUG_PRINTF("ST> ✏️  Entry %hhu of %u: %s (%hhu)\n", ++index, playlist.capacity(), playlistItem.filepath.c_str(), playlistItem.presetId);
        }
    }

    _currentState->queryStateIfNeeded();
}

void Sandtable::connected() {
    DEBUG_PRINTLN(F("Sandtable is online 🥳"));
}

void Sandtable::addToJsonState(JsonObject& root) {
    auto stateConfig = State::getConfiguration();

    JsonObject top = root.createNestedObject(FPSTR(JsonKeys::configRootKey));

    top[FPSTR(JsonKeys::configStateQueryIntervalKey)] = stateConfig->stateQueryInterval;

    top[FPSTR(JsonKeys::configIsPlaylistActiveKey)] = stateConfig->isPlaylistActive;
    top[FPSTR(JsonKeys::configDoAutoHomeKey)] = stateConfig->doAutoHome;

    top[FPSTR(JsonKeys::configPatternsFolderKey)] = stateConfig->patternsFolder;
    top[FPSTR(JsonKeys::configErasePatternsFolderKey)] = stateConfig->erasePatternsFolder;

    playlistState.writePlaylist(top);
}

void Sandtable::readFromJsonState(JsonObject& root) {
    auto stateConfig = State::getConfiguration();

    JsonObject top = root[FPSTR(JsonKeys::configRootKey)];
    if (top.isNull()) return;

    auto stateQueryInterval = top[FPSTR(JsonKeys::configStateQueryIntervalKey)];
    if (!stateQueryInterval.isNull() && stateQueryInterval.is<uint32_t>()) {
        stateConfig->stateQueryInterval = stateQueryInterval.as<uint32_t>();
    }

    auto isPlaylistActive = top[FPSTR(JsonKeys::configIsPlaylistActiveKey)];
    if (!isPlaylistActive.isNull() && isPlaylistActive.is<bool>()) {
        stateConfig->isPlaylistActive = isPlaylistActive.as<bool>();
    }

    auto doAutoHome = top[FPSTR(JsonKeys::configDoAutoHomeKey)];
    if (!doAutoHome.isNull() && doAutoHome.is<bool>()) {
        stateConfig->doAutoHome = doAutoHome.as<bool>();
    }

    auto patternFolder = top[FPSTR(JsonKeys::configPatternsFolderKey)];
    if (!patternFolder.isNull() && patternFolder.is<String>()) {
        stateConfig->patternsFolder = patternFolder.as<String>();
    }
    auto erasePatternsFolder = top[FPSTR(JsonKeys::configErasePatternsFolderKey)];
    if (!erasePatternsFolder.isNull() && erasePatternsFolder.is<String>()) {
        stateConfig->erasePatternsFolder = erasePatternsFolder.as<String>();
    }

    if (playlistState.updatePlaylist(top[FPSTR(JsonKeys::configPlaylistKey)])) {
        _writePlaylistToDebugOutput = true;

        // Write to configuration file
        serializeConfig();
    }
}

void Sandtable::addToConfig(JsonObject& root) {
    auto stateConfig = State::getConfiguration();

    JsonObject top = root.createNestedObject(FPSTR(JsonKeys::configRootKey));
    top[FPSTR(JsonKeys::configRxPinKey)] = _rxPin;
    top[FPSTR(JsonKeys::configTxPinKey)] = _txPin;

    top[FPSTR(JsonKeys::configAllowedBootTimeInSecondsKey)] = stateConfig->allowedBootUpTimeInSeconds;
    top[FPSTR(JsonKeys::configStateQueryIntervalKey)] = stateConfig->stateQueryInterval;

    top[FPSTR(JsonKeys::configIsPlaylistActiveKey)] = stateConfig->isPlaylistActive;
    top[FPSTR(JsonKeys::configDoAutoHomeKey)] = stateConfig->doAutoHome;

    top[FPSTR(JsonKeys::configPatternsFolderKey)] = stateConfig->patternsFolder;
    top[FPSTR(JsonKeys::configErasePatternsFolderKey)] = stateConfig->erasePatternsFolder;

    playlistState.writePlaylist(top);
}

bool Sandtable::readFromConfig(JsonObject& root) {
    // default settings values could be set here (or below using the 3-argument getJsonValue()) instead of in the class definition or constructor
    // setting them inside readFromConfig() is slightly more robust, handling the rare but plausible use case of single value being missing after boot (e.g. if the cfg.json was manually edited and a value was removed)
    uint8_t previousRxPin = _rxPin;
    uint8_t previousTxPin = _txPin;

    JsonObject top = root[FPSTR(JsonKeys::configRootKey)];

    // Read serial port pins
    bool configComplete = !top.isNull();

    configComplete &= getJsonValue(top[FPSTR(JsonKeys::configRxPinKey)], _rxPin, UART2_RX_PIN);
    configComplete &= getJsonValue(top[FPSTR(JsonKeys::configTxPinKey)], _txPin, UART2_TX_PIN);

    if (_rxPin != previousRxPin || _txPin != previousTxPin) {
        if (_rxPin == PIN_NOT_SET || _txPin == PIN_NOT_SET) {
            Serial2.end();

        } else {
            Serial2.begin(115200, SERIAL_8N1, _rxPin, _txPin);
        }
    }

    // Read general config
    static const SandtableConfiguration defaultStateConfig;
    auto stateConfig = State::getConfiguration();

    configComplete &= getJsonValue(top[FPSTR(JsonKeys::configStateQueryIntervalKey)], stateConfig->stateQueryInterval, defaultStateConfig.stateQueryInterval);
    configComplete &= getJsonValue(top[FPSTR(JsonKeys::configAllowedBootTimeInSecondsKey)], stateConfig->allowedBootUpTimeInSeconds, defaultStateConfig.allowedBootUpTimeInSeconds);

    configComplete &= getJsonValue(top[FPSTR(JsonKeys::configIsPlaylistActiveKey)], stateConfig->isPlaylistActive, defaultStateConfig.isPlaylistActive);
    configComplete &= getJsonValue(top[FPSTR(JsonKeys::configDoAutoHomeKey)], stateConfig->doAutoHome, defaultStateConfig.doAutoHome);

    configComplete &= getJsonValue(top[FPSTR(JsonKeys::configPatternsFolderKey)], stateConfig->patternsFolder, defaultStateConfig.patternsFolder);
    configComplete &= getJsonValue(top[FPSTR(JsonKeys::configErasePatternsFolderKey)], stateConfig->erasePatternsFolder, defaultStateConfig.erasePatternsFolder);

    // Read playlist
    playlistState.updatePlaylist(top[FPSTR(JsonKeys::configPlaylistKey)]);
    _writePlaylistToDebugOutput = true;

    return configComplete;
}