# enpm702-cpp.sh
# Source this file from your shell rc file:
#   echo "source ~/enpm702-cpp/enpm702-cpp.sh" >> ~/.bashrc   # bash users
#   echo "source ~/enpm702-cpp/enpm702-cpp.sh" >> ~/.zshrc    # zsh users

# ── Configuration ───────────────────────────────────────────────────────────
ENPM702_WS="$HOME/enpm702-cpp"
ENPM702_SRC="$ENPM702_WS/src"
ENPM702_BUILD="$ENPM702_WS/build"

# ── Bash enpm702-fall-2026-cpp/────────────────────────────────────────────────────────────────────
if [ -n "$BASH_VERSION" ]; then
    function enpm702() {
        # -- Navigate to workspace --
        cd "$ENPM702_WS" || { echo "Directory $ENPM702_WS not found"; return 1; }

        # -- Aliases for quick navigation --
        alias 702ws="cd $ENPM702_WS"
        alias 702src="cd $ENPM702_SRC"
        alias 702build="cd $ENPM702_BUILD"

        # -- Aliases for building (CMake workflow) --
        alias 702configure="cmake -S $ENPM702_SRC -B $ENPM702_BUILD -DCMAKE_BUILD_TYPE=Debug"
        alias 702release="cmake -S $ENPM702_SRC -B $ENPM702_BUILD -DCMAKE_BUILD_TYPE=Release"
        alias 702build="cmake --build $ENPM702_BUILD"
        alias 702clean="rm -rf $ENPM702_BUILD"
        alias 702rebuild="702clean && 702configure && 702build"

        # -- Alias for running executables from build --
        alias 702run="$ENPM702_BUILD/"

        # -- Quick compile a single file (useful during lectures) --
        # Usage: 702g++ myfile.cpp [-o output_name]
        alias 702g++="g++ -std=c++17 -Wall -Wextra -Wpedantic -g"

        # -- Valgrind shortcut for memory checking --
        alias 702memcheck="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"

        # -- Lecture navigation helpers --
        for i in $(seq 1 9); do
            alias "702l${i}=cd $ENPM702_SRC/lecture${i}"
        done

        echo "enpm702 C++ environment loaded"
        echo "  Workspace : $ENPM702_WS"
        echo "  Aliases   : 702ws, 702src, 702build, 702l[1-9]"
        echo "  Build     : 702configure, 702build, 702release, 702clean, 702rebuild"
        echo "  Compile   : 702g++ <file.cpp>"
        echo "  Debug     : 702memcheck <executable>"
    }
fi

# ── Zsh ─────────────────────────────────────────────────────────────────────
if [ -n "$ZSH_VERSION" ]; then
    function enpm702() {
        # -- Navigate to workspace --
        cd "$ENPM702_WS" || { echo "Directory $ENPM702_WS not found"; return 1; }

        # -- Aliases for quick navigation --
        alias 702ws="cd $ENPM702_WS"
        alias 702src="cd $ENPM702_SRC"
        alias 702build="cd $ENPM702_BUILD"

        # -- Aliases for building (CMake workflow) --
        alias 702configure="cmake -S $ENPM702_SRC -B $ENPM702_BUILD -DCMAKE_BUILD_TYPE=Debug"
        alias 702release="cmake -S $ENPM702_SRC -B $ENPM702_BUILD -DCMAKE_BUILD_TYPE=Release"
        alias 702build="cmake --build $ENPM702_BUILD"
        alias 702clean="rm -rf $ENPM702_BUILD"
        alias 702rebuild="702clean && 702configure && 702build"

        # -- Alias for running executables from build --
        alias 702run="$ENPM702_BUILD/"

        # -- Quick compile a single file (useful during lectures) --
        alias 702g++="g++ -std=c++17 -Wall -Wextra -Wpedantic -g"

        # -- Valgrind shortcut for memory checking --
        alias 702memcheck="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"

        # -- Lecture navigation helpers --
        for i in $(seq 1 9); do
            alias "702l${i}=cd $ENPM702_SRC/lecture${i}"
        done

        echo "enpm702 C++ environment loaded"
        echo "  Workspace : $ENPM702_WS"
        echo "  Aliases   : 702ws, 702src, 702build, 702l[1-9]"
        echo "  Build     : 702configure, 702build, 702release, 702clean, 702rebuild"
        echo "  Compile   : 702g++ <file.cpp>"
        echo "  Debug     : 702memcheck <executable>"
    }
fi
