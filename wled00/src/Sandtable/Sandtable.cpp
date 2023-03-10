#include "Sandtable.hpp"

void Sandtable::connected() {
    DEBUG_PRINTLN("Sandtable is online 🥳");
}

void Sandtable::loop() {
    if (millis() - lastTime > 1000) {
        //Serial.println("I'm alive!");
        lastTime = millis();
    }

    if (Serial2.available()) {
        String line = Serial2.readStringUntil('\n');

        if (line.length()) {
            // DEBUG_PRINTF("ST> %s state is processing line: %s\n", _currentState.getName().c_str(), line.c_str());

            // _currentState = _currentState.ProcessLine(line);
        }
    }
}

void Sandtable::addToConfig(JsonObject& root) {
    JsonObject top = root.createNestedObject(FPSTR(_configRootKey));
    top[FPSTR(_configRxPinKey)] = _rxPin;
    top[FPSTR(_configTxPinKey)] = _txPin;

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

    return configComplete;
}


const char Sandtable::_configRootKey[]  PROGMEM = "Sandtable";
const char Sandtable::_configRxPinKey[] PROGMEM = "Rx-pin";
const char Sandtable::_configTxPinKey[] PROGMEM = "Tx-pin";