# enpm702-cpp.sh: shortcuts for building and running the ENPM702 C++ code.
#
# =============================================================================
# SETUP: do this once on each machine (Ubuntu)
# =============================================================================
#
# 1. Install the tools.
#
#      sudo apt update
#      sudo apt install build-essential cmake gdb valgrind
#
#    Lectures that use Doxygen also need:
#
#      sudo apt install doxygen doxygen-gui graphviz
#
# 2. Clone the repository. Any folder works; this one is the default:
#
#      git clone https://github.com/zeidk/enpm702-fall-2026-cpp.git ~/enpm702_cpp/enpm702-fall-2026-cpp
#
# 3. Load this file in every new terminal, by adding one line to your shell's
#    configuration file. Use the path where YOU cloned the repository. Run
#    `echo $SHELL` if you do not know which shell you use.
#
#      bash:  echo 'source ~/enpm702_cpp/enpm702-fall-2026-cpp/enpm702-cpp.sh' >> ~/.bashrc
#      zsh:   echo 'source ~/enpm702_cpp/enpm702-fall-2026-cpp/enpm702-cpp.sh' >> ~/.zshrc
#
#    Run that line once only: each run adds another copy to the file.
#
# 4. Open a new terminal (or run `source ~/.bashrc` or `source ~/.zshrc`), then:
#
#      enpm702          # go to the repository and load the 702* shortcuts
#      702configure     # once, and again after a CMakeLists.txt changes
#      702build         # compile everything
#      702exe           # list the programs that were built
#      702run week5_snippets
#
#    Type 702help for the full list of shortcuts.
#
# Notes
#   - Source this file; do not run it. `bash enpm702-cpp.sh` loads nothing
#     into your terminal, so the file stops with a message if you try.
#   - The file finds the repository from its own location, so the clone can be
#     anywhere. To point it somewhere else, set ENPM702_WS before the source
#     line in your configuration file:
#       export ENPM702_WS="$HOME/somewhere/enpm702-fall-2026-cpp"
#   - After you `git pull`, open a new terminal so the latest version of this
#     file is loaded.
#
# Repository layout assumed by this script:
#   enpm702-fall-2026-cpp/
#   |-- CMakeLists.txt        <- top-level, adds project/weekN
#   |-- enpm702-cpp.sh        <- this file
#   |-- project/
#   |   |-- week1/
#   |   |   |-- CMakeLists.txt
#   |   |   `-- src/
#   |   `-- ... week9/
#   `-- build/                <- created by 702configure, git-ignored

# --- Sourced, not run ---------------------------------------------------------
# Running this file starts a new shell, defines everything there, and exits:
# nothing reaches the terminal you typed in. Stop with a message instead.
_enpm702_sourced=1
if [ -n "${BASH_VERSION:-}" ] && [ "${BASH_SOURCE[0]}" = "$0" ]; then
    _enpm702_sourced=0
fi
if [ -n "${ZSH_VERSION:-}" ]; then
    case "$ZSH_EVAL_CONTEXT" in
        *:file*) ;;
        *) _enpm702_sourced=0 ;;
    esac
fi
if [ "$_enpm702_sourced" = 0 ]; then
    echo "enpm702-cpp.sh must be sourced, not run:"
    echo "    source $0"
    echo "See the SETUP notes at the top of the file."
    exit 1
fi
unset _enpm702_sourced

# --- Configuration ----------------------------------------------------------
# The repository is the folder that holds this file, unless ENPM702_WS is
# already set. bash and zsh name the file being sourced differently.
if [ -z "${ENPM702_WS:-}" ]; then
    _enpm702_self=""
    if [ -n "${BASH_VERSION:-}" ]; then
        _enpm702_self="${BASH_SOURCE[0]}"
    elif [ -n "${ZSH_VERSION:-}" ]; then
        eval '_enpm702_self="${(%):-%x}"'
    fi
    if [ -n "$_enpm702_self" ]; then
        # Follow a symlink to the real file, so its folder is the repository.
        _enpm702_self="$(readlink -f "$_enpm702_self" 2>/dev/null || echo "$_enpm702_self")"
        ENPM702_WS="$(cd "$(dirname "$_enpm702_self")" && pwd -P)"
    else
        ENPM702_WS="$HOME/enpm702_cpp/enpm702-fall-2026-cpp"
    fi
    unset _enpm702_self
fi
ENPM702_PROJECT="$ENPM702_WS/project"
ENPM702_BUILD="$ENPM702_WS/build"
ENPM702_WEEKS=9
ENPM702_STD="c++20"     # keep in step with CMAKE_CXX_STANDARD in CMakeLists.txt

# --- Helpers ----------------------------------------------------------------
# Defined at source time with a _enpm702_ prefix so that the short 702* names
# can stay aliases. This keeps one implementation for both bash and zsh.

# Check that a program is installed; if not, say which package provides it.
# Usage: _enpm702_need cmake cmake
_enpm702_need() {
    if ! command -v "$1" > /dev/null 2>&1; then
        echo "$1 is not installed. Install it with: sudo apt install $2"
        return 1
    fi
}

# Configure the build tree at the repository root.
# Usage (alias):  702configure                  # CMAKE_BUILD_TYPE=Debug
#                 702release                    # CMAKE_BUILD_TYPE=Release
# Usage (direct): _enpm702_configure RelWithDebInfo
_enpm702_configure() {
    _enpm702_need cmake cmake || return 1
    _enpm702_need g++ build-essential || return 1
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
    # A bare target name becomes --target <name>; options pass through as given.
    if [ "$#" -ge 1 ] && [ "${1#-}" = "$1" ]; then
        local target="$1"
        shift
        cmake --build "$ENPM702_BUILD" --target "$target" "$@"
    else
        cmake --build "$ENPM702_BUILD" "$@"
    fi
}

# Delete the whole build tree. Refuses any path that is not a build/ folder,
# so a wrong ENPM702_WS can never make it delete something else.
# Usage: 702clean
_enpm702_clean() {
    case "$ENPM702_BUILD" in
        */build) ;;
        *)
            echo "Refusing to delete '$ENPM702_BUILD': it is not a build/ folder."
            return 1
            ;;
    esac
    if [ ! -d "$ENPM702_BUILD" ]; then
        echo "Nothing to remove: $ENPM702_BUILD does not exist."
        return 0
    fi
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
    if [ ! -d "$ENPM702_BUILD" ]; then
        echo "No build directory yet. Run 702configure, then 702build."
        return 1
    fi
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
        echo "Did you build it? Try 702build $target"
        return 1
    fi

    "$exe" "$@"
}

# Check a program for memory errors with Valgrind.
# Usage: 702memcheck ./hello
#        702memcheck build/project/week3/week3_main
_enpm702_memcheck() {
    _enpm702_need valgrind valgrind || return 1
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes "$@"
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
    if [ ! -f "$ENPM702_WS/CMakeLists.txt" ] || [ ! -d "$ENPM702_PROJECT" ]; then
        echo "No ENPM702 repository at $ENPM702_WS"
        echo "Set ENPM702_WS to the folder you cloned, before the source line in"
        echo "your ~/.bashrc or ~/.zshrc. See the SETUP notes in enpm702-cpp.sh."
        return 1
    fi
    cd "$ENPM702_WS" || return 1

    # -- Navigation --
    # Usage: 702ws | 702proj | 702bin
    alias 702ws="cd '$ENPM702_WS'"
    alias 702proj="cd '$ENPM702_PROJECT'"
    alias 702bin="cd '$ENPM702_BUILD'"

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
    # The same warnings as the CMake build, so both report the same problems.
    # Usage: 702g++ hello.cpp -o hello
    alias 702g++="g++ -std=${ENPM702_STD} -Wall -Wextra -pedantic-errors -Wshadow -g"

    # -- Valgrind shortcut for memory checking --
    # Usage: 702memcheck ./hello
    alias 702memcheck="_enpm702_memcheck"

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
