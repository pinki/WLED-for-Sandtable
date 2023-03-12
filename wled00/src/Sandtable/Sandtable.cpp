#include "Sandtable.hpp"

void Sandtable::setup() {
    lastTime = millis();
}

void Sandtable::loop() {
    if (Serial2.available()) {
        lastTime = millis();

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

    } else if (_stateQueryInterval > 0 && millis() - lastTime > _stateQueryInterval) {
        lastTime = millis();

        DEBUG_PRINTLN(F("ST> Querying state"));
        Serial2.println(FPSTR(GCode::StateCommand));
    }
}

void Sandtable::connected() {
    DEBUG_PRINTLN(F("Sandtable is online 🥳"));
}

void Sandtable::addToConfig(JsonObject& root) {
    auto stateConfig = State::getConfiguration();

    JsonObject top = root.createNestedObject(FPSTR(_configRootKey));
    top[FPSTR(_configRxPinKey)] = _rxPin;
    top[FPSTR(_configTxPinKey)] = _txPin;

    top[FPSTR(_configAllowedBootTimeInSecondsKey)] = stateConfig->allowedBootUpTimeInSeconds;
    top[FPSTR(_configStateQueryIntervalKey)] = _stateQueryInterval;

    top[FPSTR(_configIsPlaylistActiveKey)] = stateConfig->isPlaylistActive;
    top[FPSTR(_configDoAutoHomeKey)] = stateConfig->doAutoHome;

    // JsonArray pinArray = top.createNestedArray(FPSTR(_pinsSection));
    // pinArray.add(_rxPin);
    // pinArray.add(_txPin); 
}

bool Sandtable::readFromConfig(JsonObject& root) {
    // default settings values could be set here (or below using the 3-argument getJsonValue()) instead of in the class definition or constructor
    // setting them inside readFromConfig() is slightly more robust, handling the rare but plausible use case of single value being missing after boot (e.g. if the cfg.json was manually edited and a value was removed)
    uint8_t previousRxPin = _rxPin;
    uint8_t previousTxPin = _txPin;

    JsonObject top = root[FPSTR(_configRootKey)];

    bool configComplete = !top.isNull();

    configComplete &= getJsonValue(top[FPSTR(_configRxPinKey)], _rxPin, UART2_RX_PIN);
    configComplete &= getJsonValue(top[FPSTR(_configTxPinKey)], _txPin, UART2_TX_PIN);

    // configComplete &= getJsonValue(top[FPSTR(_pinsSection)][0], _rxPin, UART2_RX_PIN);
    // configComplete &= getJsonValue(top[FPSTR(_pinsSection)][1], _txPin, UART2_TX_PIN);

    if (_rxPin != previousRxPin || _txPin != previousTxPin) {
        if (_rxPin == NO_PIN || _txPin == NO_PIN) {
            Serial2.end();

        } else {
            Serial2.begin(115200, SERIAL_8N1, _rxPin, _txPin);
        }
    }

    static const SandtableConfiguration defaultStateConfig;
    auto stateConfig = State::getConfiguration();

    configComplete &= getJsonValue(top[FPSTR(_configStateQueryIntervalKey)], _stateQueryInterval, 3000);
    configComplete &= getJsonValue(top[FPSTR(_configAllowedBootTimeInSecondsKey)], stateConfig->allowedBootUpTimeInSeconds, defaultStateConfig.allowedBootUpTimeInSeconds);

    configComplete &= getJsonValue(top[FPSTR(_configIsPlaylistActiveKey)], stateConfig->isPlaylistActive, defaultStateConfig.isPlaylistActive);
    configComplete &= getJsonValue(top[FPSTR(_configDoAutoHomeKey)], stateConfig->doAutoHome, defaultStateConfig.doAutoHome);

    return configComplete;
}


const char Sandtable::_configRootKey[]                     PROGMEM = "Sandtable";
const char Sandtable::_configRxPinKey[]                    PROGMEM = "Rx-pin";
const char Sandtable::_configTxPinKey[]                    PROGMEM = "Tx-pin";
const char Sandtable::_configStateQueryIntervalKey[]       PROGMEM = "StateQueryInterval";
const char Sandtable::_configIsPlaylistActiveKey[]         PROGMEM = "IsPlaylistActive";
const char Sandtable::_configDoAutoHomeKey[]               PROGMEM = "DoAutoHome";
const char Sandtable::_configAllowedBootTimeInSecondsKey[] PROGMEM = "AllowedBootTimeInSeconds";