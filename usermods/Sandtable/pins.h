#pragma once

#include "fastpin.h"

#ifdef ESP32
    #define UART2_RX_PIN    16
    #define UART2_TX_PIN    17
#endif

#if !defined(UART2_RX_PIN) && !defined(UART2_TX_PIN)
    #define UART2_RX_PIN    NO_PIN
    #define UART2_TX_PIN    NO_PIN
#endif