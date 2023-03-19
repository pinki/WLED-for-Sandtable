#include <pgmspace.h>

#include "JsonKeys.hpp"

using namespace SandtableUsermod;

const char JsonKeys::configRootKey[]                     PROGMEM = "Sandtable";
const char JsonKeys::configRxPinKey[]                    PROGMEM = "Rx-pin";
const char JsonKeys::configTxPinKey[]                    PROGMEM = "Tx-pin";
const char JsonKeys::configStateQueryIntervalKey[]       PROGMEM = "StateQueryInterval";
const char JsonKeys::configIsPlaylistActiveKey[]         PROGMEM = "IsPlaylistActive";
const char JsonKeys::configDoAutoHomeKey[]               PROGMEM = "DoAutoHome";
const char JsonKeys::configAllowedBootTimeInSecondsKey[] PROGMEM = "AllowedBootTimeInSeconds";
const char JsonKeys::configPatternsFolderKey[]           PROGMEM = "PatternsFolder";
const char JsonKeys::configErasePatternsFolderKey[]      PROGMEM = "ErasePatternsFolder";
const char JsonKeys::configPlaylistKey[]                 PROGMEM = "Playlist";
const char JsonKeys::configPlaylistItemEraseAfterKey[]   PROGMEM = "EraseAfter";
const char JsonKeys::configPlaylistItemPresetIdKey[]     PROGMEM = "PresetId";
const char JsonKeys::configPlaylistItemFilepathKey[]     PROGMEM = "Filepath";