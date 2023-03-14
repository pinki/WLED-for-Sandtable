#pragma once

#include "wled.h"
#include "../gcodes.hpp"

namespace SandtableUsermod {
    typedef enum MotorPowerState {
        Unknown,
        On,
        Off
    } MotorPowerState;

    typedef enum CommandState {
        NotSent,
        Sent,
        Acknowledged
    } CommandState;

    typedef struct SandtableConfiguration {
        bool isPlaylistActive = true;
        bool doAutoHome = true;
        uint8_t allowedBootUpTimeInSeconds = 15;
        String patternsFolder = "sdcard";
        String erasePatternsFolder = "sdcard/Erasers";
    } SandtableConfiguration;

    class State {
        protected:
            unsigned long _activeSince;
            static MotorPowerState _motorPowerState;
            static SandtableConfiguration _configuration;

            static const char OkLine[];
            static const char NewStatePrintfDebugLine[];

        public:
            State();
            virtual ~State() { }

            virtual State* ProcessLine(const String& line) = 0;
            virtual const char* getName() = 0;

            virtual void activate();

            static SandtableConfiguration* const getConfiguration();
    };
};