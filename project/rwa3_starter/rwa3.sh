#!/usr/bin/env bash
# RWA3: build, run, check and zip your project from a terminal.
#
# Run it from anywhere; it works in the folder it is in:
#
#   ./rwa3.sh build   configure (the first time) and compile
#   ./rwa3.sh run     compile, then run ./build/rwa3
#   ./rwa3.sh check   rebuild from scratch, count the warnings, run under Valgrind
#   ./rwa3.sh docs    generate the Doxygen pages in docs/html
#   ./rwa3.sh zip     make ../<folder>.zip for Canvas
#   ./rwa3.sh clean   delete build/, docs/html and docs/latex
#
# It uses only cmake, g++, valgrind, doxygen and zip, and nothing from your
# ~/.bashrc or ~/.zshrc.

set -euo pipefail

cd "$(dirname "$(readlink -f "$0")")"
project_dir="$(pwd)"
folder="$(basename "$project_dir")"

need() {
    if ! command -v "$1" > /dev/null 2>&1; then
        echo "$1 is not installed. Install it with: sudo apt install $2"
        exit 1
    fi
}

build() {
    need cmake cmake
    need g++ build-essential
    if [ ! -f build/CMakeCache.txt ]; then
        cmake -S . -B build
    fi
    cmake --build build "$@"
}

case "${1:-}" in
    build)
        build
        ;;
    run)
        build
        echo "----- ./build/rwa3 -----"
        ./build/rwa3
        ;;
    check)
        build --clean-first 2>&1 | tee build/last_build.log
        warnings="$(grep -c 'warning:' build/last_build.log || true)"
        echo "----- warnings in the build: $warnings -----"
        need valgrind valgrind
        valgrind --leak-check=full --error-exitcode=1 ./build/rwa3
        echo "----- Valgrind: no error. Paste its last lines into README.md -----"
        ;;
    docs)
        need doxygen doxygen
        (cd docs && doxygen Doxyfile > /dev/null)
        echo "Doxygen pages: docs/html/index.html"
        ;;
    zip)
        need zip zip
        if [ "$folder" = "rwa3_starter" ]; then
            echo "Rename the folder first: rwa3_firstname_lastname"
            exit 1
        fi
        if [ -f include/README.md ]; then
            echo "Delete include/README.md before you submit."
            exit 1
        fi
        cd ..
        rm -f "$folder.zip"
        zip -r -q "$folder.zip" "$folder" \
            -x "$folder/build/*" "$folder/docs/html/*" "$folder/docs/latex/*" \
               "$folder/.vscode/*"
        echo "Created $(pwd)/$folder.zip:"
        zip -sf "$folder.zip"
        ;;
    clean)
        rm -rf build docs/html docs/latex
        echo "Deleted build/, docs/html and docs/latex"
        ;;
    *)
        echo "Usage: ./rwa3.sh build | run | check | docs | zip | clean"
        exit 1
        ;;
esac
