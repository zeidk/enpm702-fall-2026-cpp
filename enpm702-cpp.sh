# enpm702-cpp.sh
# Source this file from your shell rc file:
#   echo "source ~/enpm702_cpp/enpm702-fall-2026-cpp/enpm702-cpp.sh" >> ~/.bashrc   # bash users
#   echo "source ~/enpm702_cpp/enpm702-fall-2026-cpp/enpm702-cpp.sh" >> ~/.zshrc    # zsh users
#
# Then run `enpm702` in any shell to load the environment.
#
# Repository layout assumed by this script:
#   enpm702-fall-2026-cpp/
#   |-- CMakeLists.txt        <- top-level, adds project/weekN
#   |-- project/
#   |   |-- week1/
#   |   |   |-- CMakeLists.txt
#   |   |   `-- src/
#   |   `-- ... week9/
#   `-- build/                <- created by 702configure, git-ignored

# --- Configuration ----------------------------------------------------------
ENPM702_WS="$HOME/enpm702_cpp/enpm702-fall-2026-cpp"
ENPM702_PROJECT="$ENPM702_WS/project"
ENPM702_BUILD="$ENPM702_WS/build"
ENPM702_WEEKS=9
ENPM702_STD="c++20"

# --- Helpers ----------------------------------------------------------------
# Defined at source time with a _enpm702_ prefix so that the short 702* names
# can stay aliases. This keeps one implementation for both bash and zsh.

# Configure the build tree at the repository root.
# Usage (alias):  702configure                  # CMAKE_BUILD_TYPE=Debug
#                 702release                    # CMAKE_BUILD_TYPE=Release
# Usage (direct): _enpm702_configure RelWithDebInfo
_enpm702_configure() {
    cmake -S "$ENPM702_WS" -B "$ENPM702_BUILD" \
          -DCMAKE_BUILD_TYPE="${1:-Debug}" \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
}

# Build everything, a single target, or pass any option through to CMake.
# Usage: 702build                                # build all targets
#        702build week3_main                     # build one target
#        702build -j 8                           # build all targets on 8 jobs
#        702build --target week3_main -j 8
_enpm702_build() {
    if [ ! -d "$ENPM702_BUILD" ]; then
        echo "No build directory yet. Run 702configure first."
        return 1
    fi
    cmake --build "$ENPM702_BUILD" "$@"
}

# Delete the whole build tree.
# Usage: 702clean
_enpm702_clean() {
    rm -rf "$ENPM702_BUILD"
    echo "Removed $ENPM702_BUILD"
}

# Clean, configure in Debug, then build everything from scratch.
# Usage: 702rebuild
_enpm702_rebuild() {
    _enpm702_clean && _enpm702_configure Debug && _enpm702_build
}

# Change directory to a given week under project/.
# Usage (alias):  702w3                          # cd project/week3
#                 702w7                          # cd project/week7
# Usage (direct): _enpm702_week 3
_enpm702_week() {
    local n="$1"
    local dir="$ENPM702_PROJECT/week${n}"
    if [ ! -d "$dir" ]; then
        echo "Directory $dir not found"
        return 1
    fi
    cd "$dir" || return 1
}

# List the executables that currently exist under build/.
# Usage: 702exe
#        702exe | grep week4
_enpm702_exe() {
    if [ ! -d "$ENPM702_BUILD" ]; then
        echo "No build directory yet. Run 702configure first."
        return 1
    fi
    find "$ENPM702_BUILD" -type f -perm -u+x \
         -not -name '*.cmake' -not -name '*.sh' \
         -not -path '*/CMakeFiles/*' | sort
}

# Run an executable by name, wherever CMake placed it under build/.
# Usage: 702run week3_main                       # run with no arguments
#        702run week5_sensors 10 0.5             # forward arguments to the program
#        702run                                  # no name given, print what is available
_enpm702_run() {
    if [ "$#" -eq 0 ]; then
        echo "usage: 702run <executable> [args...]"
        echo "available:"
        _enpm702_exe
        return 2
    fi

    local target="$1"
    shift

    local exe
    exe=$(find "$ENPM702_BUILD" -type f -perm -u+x -name "$target" \
               -not -path '*/CMakeFiles/*' 2>/dev/null | head -n 1)

    if [ -z "$exe" ]; then
        echo "Executable '$target' not found under $ENPM702_BUILD"
        echo "Did you build it? Try 702build"
        return 1
    fi

    "$exe" "$@"
}

# Print the list of available commands.
# Usage: 702help
_enpm702_help() {
    echo "enpm702 C++ environment"
    echo "  Workspace : $ENPM702_WS"
    echo "  Navigate  : 702ws, 702proj, 702bin, 702w[1-${ENPM702_WEEKS}]"
    echo "  Build     : 702configure, 702release, 702build [target], 702clean, 702rebuild"
    echo "  Run       : 702run <executable> [args], 702exe"
    echo "  Compile   : 702g++ <file.cpp> [-o name]   (single file, ${ENPM702_STD})"
    echo "  Debug     : 702memcheck <executable>"
    echo "  Help      : 702help"
}

# --- Entry point ------------------------------------------------------------
# Load the environment: cd to the workspace and install the 702* shortcuts.
# Usage: enpm702
enpm702() {
    cd "$ENPM702_WS" || { echo "Directory $ENPM702_WS not found"; return 1; }

    # -- Navigation --
    # Usage: 702ws | 702proj | 702bin
    alias 702ws="cd $ENPM702_WS"
    alias 702proj="cd $ENPM702_PROJECT"
    alias 702bin="cd $ENPM702_BUILD"

    # -- CMake workflow --
    # Usage: 702configure | 702release | 702build [target] | 702clean | 702rebuild
    alias 702configure="_enpm702_configure Debug"
    alias 702release="_enpm702_configure Release"
    alias 702build="_enpm702_build"
    alias 702clean="_enpm702_clean"
    alias 702rebuild="_enpm702_rebuild"

    # -- Running --
    # Usage: 702run week3_main [args...] | 702exe
    alias 702run="_enpm702_run"
    alias 702exe="_enpm702_exe"

    # -- Quick compile of a single file (useful during lectures) --
    # Usage: 702g++ hello.cpp -o hello
    alias 702g++="g++ -std=${ENPM702_STD} -Wall -Wextra -Wpedantic -g"

    # -- Valgrind shortcut for memory checking --
    # Usage: 702memcheck ./hello
    alias 702memcheck="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"

    # -- Week navigation --
    # Usage: 702w1 ... 702w9
    local i
    for i in $(seq 1 "$ENPM702_WEEKS"); do
        alias "702w${i}=_enpm702_week ${i}"
    done

    # Usage: 702help
    alias 702help="_enpm702_help"

    _enpm702_help
}