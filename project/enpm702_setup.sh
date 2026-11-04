#!/usr/bin/env bash
# enpm702_setup.sh: build and run the live coding programs of L8 and L9, and the
# week 9 playground.
#
# The script sits in the project/ folder of the course repository and finds the
# repository from there, so it works wherever you cloned it. Run every command
# below from the repository's top folder.
#
# Before class, run it once. It checks that each program compiles and runs:
#     bash project/enpm702_setup.sh        # weeks 8 and 9
#     bash project/enpm702_setup.sh 9      # week 9 only
#
# In class, load it into your terminal once, then type a command after each step:
#     source project/enpm702_setup.sh
#     live8                        # build week8_live; if it compiles, run it
#     live9                        # the same for week9_live
#     playground9                  # build week9_playground; run the code of every slide
#     playground9 77               # run only the code of [Slide 77]
#
# It changes no file in the repository.

# The repository is the folder above the one that holds this script. bash and zsh
# each have their own way to name the file being sourced.
if [ -n "${BASH_VERSION:-}" ]; then
    _enpm702_self="${BASH_SOURCE[0]}"
elif [ -n "${ZSH_VERSION:-}" ]; then
    eval '_enpm702_self="${(%):-%x}"'
else
    _enpm702_self="$0"
fi
ENPM702_WS="$(cd "$(dirname "$_enpm702_self")/.." && pwd)"
unset _enpm702_self

# Build the CMake target $1. The compiler's output goes to the file $2; returns 1 to
# 3 for each kind of failure, 0 when the program is built. When CMake does not know
# the target yet (a folder just added to the repository), it configures again once
# and retries.
_enpm702_build() {
    local target="$1"
    if [ ! -f "$ENPM702_WS/CMakeLists.txt" ]; then
        return 1
    fi
    if [ ! -f "$ENPM702_WS/build/CMakeCache.txt" ]; then
        cmake -S "$ENPM702_WS" -B "$ENPM702_WS/build" -DCMAKE_BUILD_TYPE=Debug \
              -DCMAKE_EXPORT_COMPILE_COMMANDS=ON > "$2" 2>&1 || return 2
    fi
    if cmake --build "$ENPM702_WS/build" --target "$target" > "$2" 2>&1; then
        return 0
    fi
    if grep -q "No rule to make target" "$2"; then
        cmake -S "$ENPM702_WS" -B "$ENPM702_WS/build" > "$2" 2>&1 || return 2
        cmake --build "$ENPM702_WS/build" --target "$target" > "$2" 2>&1 || return 3
        return 0
    fi
    return 3
}

# Print the path of the program $1 under build/.
_enpm702_exe() {
    find "$ENPM702_WS/build" -type f -perm -u+x -name "$1" \
         -not -path '*/CMakeFiles/*' 2>/dev/null | head -n 1
}

# Build the program $1 and, if it compiles, run it with the other arguments.
_enpm702_run() {
    local target="$1" log exe rc
    shift
    log="$(mktemp)"
    _enpm702_build "$target" "$log"
    rc=$?
    if [ "$rc" -eq 1 ]; then
        echo "No ENPM702 repository at $ENPM702_WS."
        echo "Keep this script in the project/ folder of the course repository."
    elif [ "$rc" -ne 0 ]; then
        cat "$log"
        echo "$target did not compile: read the first error above."
    fi
    rm -f "$log"
    [ "$rc" -eq 0 ] || return 1
    exe="$(_enpm702_exe "$target")"
    "$exe" "$@"
}

# week9_playground needs common/, which holds the finished classes of the Live
# Coding steps and joins the repository after the Live Coding deadline.
_enpm702_has_playground9() {
    [ -d "$ENPM702_WS/project/week9/common" ] && [ -d "$ENPM702_WS/project/week9/playground" ]
}

live8() { _enpm702_run week8_live; }
live9() { _enpm702_run week9_live; }
playground9() {
    if ! _enpm702_has_playground9; then
        echo "The week 9 playground is not in your copy of the repository yet."
        echo "It joins after the Live Coding deadline: pull the repository then."
        return 1
    fi
    _enpm702_run week9_playground "$@"
}

# --- Sourced: the commands are now defined in this terminal ------------------
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
if [ "$_enpm702_sourced" = 1 ]; then
    unset _enpm702_sourced
    echo "live8, live9 and playground9 are ready in this terminal."
    return 0
fi

# --- Run with bash: check that the programs compile and run ------------------
case "${1:-}" in
    '') weeks="8 9" ;;
    *[!0-9]*) echo "usage: bash project/enpm702_setup.sh [week number]"; exit 2 ;;
    *) weeks="$1" ;;
esac

# The programs to check: each week's live coding program, and the week 9
# playground when its folders are in the repository.
targets=""
for week in $weeks; do
    targets="$targets week${week}_live"
    if [ "$week" = 9 ] && _enpm702_has_playground9; then
        targets="$targets week9_playground"
    fi
done

# Colors and symbols in a terminal; plain text in a file, a pipe, or with NO_COLOR.
if [ -t 1 ] && [ -z "${NO_COLOR:-}" ]; then
    bold=$'\e[1m'; dim=$'\e[2m'; red=$'\e[31m'; green=$'\e[32m'
    yellow=$'\e[33m'; cyan=$'\e[36m'; reset=$'\e[0m'; clear_line=$'\r\e[K'
    ok_mark="${green}✔${reset}"; fix_mark="${red}✘${reset}"
    tty=1
else
    bold=""; dim=""; red=""; green=""; yellow=""; cyan=""; reset=""; clear_line=""
    ok_mark="ok "; fix_mark="FIX"
    tty=0
fi

ok()   { echo "    $ok_mark $*"; }
fix()  { echo "    $fix_mark ${red}$*${reset}"; }
show_log() {
    echo "      ${dim}last lines of the output:${reset}"
    tail -n 15 "$1" | sed "s/^/      ${dim}│${reset} /"
}

repo_shown="${ENPM702_WS/#$HOME/\~}"
echo
echo "  ${cyan}╭──────────────────────────────────────────────╮${reset}"
echo "  ${cyan}│${reset}  ${bold}ENPM702${reset} · Setup check                       ${cyan}│${reset}"
echo "  ${cyan}╰──────────────────────────────────────────────╯${reset}"
echo
echo "  ${bold}Repository${reset}  $repo_shown"

if [ ! -f "$ENPM702_WS/CMakeLists.txt" ]; then
    fix "no ENPM702 repository above this script"
    echo "      Keep this script in the project/ folder of the course repository."
    echo
    exit 1
fi
ok "repository found"

failed=0
for target in $targets; do
    log="$(mktemp)"
    echo
    echo "  ${bold}$target${reset}"
    [ "$tty" = 1 ] && printf "    ${yellow}…${reset} building"
    start=$SECONDS
    _enpm702_build "$target" "$log"
    rc=$?
    seconds=$((SECONDS - start))
    printf "%s" "$clear_line"
    if [ "$rc" -eq 2 ]; then
        fix "CMake could not configure the repository"
        show_log "$log"; failed=1; rm -f "$log"; continue
    elif [ "$rc" -eq 3 ]; then
        fix "$target did not build"
        show_log "$log"; failed=1; rm -f "$log"; continue
    fi
    ok "built $target ${dim}(${seconds} s)${reset}"
    exe="$(_enpm702_exe "$target")"
    if [ -n "$exe" ] && "$exe" > "$log" 2>&1; then
        ok "ran $target"
    else
        fix "$target did not run"
        show_log "$log"; failed=1
    fi
    rm -f "$log"
done

echo
if [ "$failed" -ne 0 ]; then
    echo "  $fix_mark ${bold}Fix the lines marked above, then run this script again.${reset}"
    echo
    exit 1
fi
echo "  $ok_mark ${bold}Ready for class.${reset}"
echo
echo "  In class, type this once in your terminal, from the repository's top folder:"
echo "      ${cyan}source project/enpm702_setup.sh${reset}"
echo "  Then, after each step:"
for week in $weeks; do
    echo "      ${cyan}live$week${reset}            ${dim}Lecture $week${reset}"
done
case " $targets " in
    *" week9_playground "*)
        echo "      ${cyan}playground9 77${reset}   ${dim}the code of [Slide 77] of Lecture 9${reset}" ;;
esac
echo
exit 0
