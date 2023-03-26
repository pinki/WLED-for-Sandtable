#pragma once

#include "wled.h"
#include "../gcodes.hpp"

namespace SandtableUsermod {
    enum MotorPowerState {
        Unknown,
        On,
        Off
    };

    enum CommandState {
        NotSent,
        Sent,
        Acknowledged
    };

    struct SandtableConfiguration {
        bool isPlaylistActive = true;
        bool doAutoHome = true;
        uint8_t allowedBootUpTimeInSeconds = 15;
        uint16_t stateQueryInterval = 5000;
        String patternsFolder = "sdcard";
        String erasePatternsFolder = "sdcard/Erasers";
    };

    class State {
        protected:
            unsigned long _activeSince;
            static MotorPowerState _motorPowerState;
            static SandtableConfiguration _configuration;

            static unsigned long _lastProcessedLineAt;
            static CommandState _queryStateCommandState;

            static const char OkLine[];
            static const char MessageLineStart[];
            static const char NewStatePrintfDebugLine[];

            bool isLineOkForStateQueryCommand(const String& line);

        public:
            State();
            virtual ~State() { }

            virtual State* ProcessLine(const String& line) = 0;
            virtual const char* getName() = 0;

            virtual void activate();

            virtual void queryStateIfNeeded();

            static SandtableConfiguration* const getConfiguration();
    };
};