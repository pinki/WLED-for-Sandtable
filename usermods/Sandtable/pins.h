#pragma once

#include "fastpin.h"

#define PIN_NOT_SET   -1

#ifdef ESP32
    #define UART2_RX_PIN    16
    #define UART2_TX_PIN    17
#endif

#if !defined(UART2_RX_PIN) && !defined(UART2_TX_PIN)
    #define UART2_RX_PIN    PIN_NOT_SET
    #define UART2_TX_PIN    PIN_NOT_SET
#endif