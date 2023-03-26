#pragma once

namespace SandtableUsermod {
    class GCode {
        public:
            static const char HomeCommand[];
            static const char StateCommand[];
            static const char RebootSystemCommand[];
            static const char DisableMotorsCommand[];
            static const char RunSDCardFileCommand[];
            static const char RunLocalFSFileCommand[];
            static const char ListSDCardFileCommand[];
            static const char ListLocalFSFileCommand[];
    };
};
