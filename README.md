# ENPM702 - Fall 2026 - C++

C++ code snippets from lecture slides.

## Requirements

- A C++20 compiler (`g++` 10 or newer, `g++` 13 recommended)
- `cmake` 3.16 or newer
- `valgrind` (optional, only for `702memcheck`)

## Repository layout

```text
enpm702-fall-2026-cpp/
├── CMakeLists.txt        # top-level, adds project/weekN
├── project/
│   ├── week1/
│   │   ├── CMakeLists.txt
│   │   └── src/
│   ├── week2/
│   ├── week5/            # two halves, so it is bigger than the others
│   │   ├── CMakeLists.txt
│   │   ├── playground/   # the slide snippets, one file, target week5_snippets
│   │   │   └── src/
│   │   └── arm_demo/     # the same program, complete and documented
│   │       ├── include/  # headers: the declarations and their comments
│   │       ├── src/      # source files: the definitions, and main.cpp
│   │       └── docs/     # Doxyfile, and the html/ pages it generates
│   └── ... week9/
├── enpm702-cpp.sh
├── LICENSE
└── README.md
```

A week is a single `src/main.cpp` until the lecture needs more. Week 5 has two
halves. `playground/src/snippets.cpp` holds every snippet from the slides
(target `week5_snippets`). `arm_demo/` is the same program split into headers
and source files and documented with Doxygen (target `week5_arm_demo`). The
Header Files slides, Exercise 1 and the Documenting Functions section use it.

The demo is **not built by default**. When you reach the Header Files section
you uncomment the last four lines of `project/week5/CMakeLists.txt`, which is
the one edit that turns it on.

The `build/` directory is created by `702configure` and is git-ignored, and so
is `docs/html/`. Commit the `Doxyfile`, never the pages it generates.

## Setup

The same steps are at the top of `enpm702-cpp.sh`.

### 0. Install the tools (Ubuntu)

```bash
sudo apt update
sudo apt install build-essential cmake gdb valgrind
```

### 1. Clone the repository

```bash
git clone https://github.com/zeidk/enpm702-fall-2026-cpp.git ~/enpm702_cpp/enpm702-fall-2026-cpp
```

Any folder works: `enpm702-cpp.sh` finds the repository from its own location.
If you clone it somewhere else, use that path in the `source` line below. To
point the shortcuts at a different clone, set `ENPM702_WS` before the `source`
line:

```bash
export ENPM702_WS="$HOME/somewhere/enpm702-fall-2026-cpp"
```

### 2. Source the shell script

Add the following line to your shell configuration file:

**Bash users:**

```bash
echo "source ~/enpm702_cpp/enpm702-fall-2026-cpp/enpm702-cpp.sh" >> ~/.bashrc
```

**Zsh users:**

```bash
echo "source ~/enpm702_cpp/enpm702-fall-2026-cpp/enpm702-cpp.sh" >> ~/.zshrc
```

Run that line once only: each run adds another copy to the file. Then restart
your terminal or run `source ~/.bashrc` (or `source ~/.zshrc`).

Source the script; do not run it. `bash enpm702-cpp.sh` loads nothing into your
terminal, so the script stops with a message if you try.

### 3. Activate the environment

```bash
enpm702
```

This moves you to the workspace root and loads the navigation, build, and run
shortcuts into your shell. Run it once per terminal session. Type `702help` at
any time to print the list of commands again.

## Available commands

### Navigation

| Command | Description |
|---|---|
| `702ws` | Navigate to the workspace root |
| `702proj` | Navigate to `project/` |
| `702bin` | Navigate to `build/` |
| `702w1` ... `702w9` | Navigate to `project/week<N>/` |

### Build

| Command | Description |
|---|---|
| `702configure` | Configure the build tree (`CMAKE_BUILD_TYPE=Debug`) |
| `702release` | Configure the build tree (`CMAKE_BUILD_TYPE=Release`) |
| `702build` | Build every target |
| `702build <target>` | Build a single target |
| `702clean` | Remove the build directory |
| `702rebuild` | Clean, configure in Debug, then build |

### Run and debug

| Command | Description |
|---|---|
| `702exe` | List the executables currently present under `build/` |
| `702run <exe> [args]` | Run an executable by name, wherever CMake placed it |
| `702g++ <file.cpp>` | Compile a single file (`g++ -std=c++20 -Wall -Wextra -pedantic-errors -Wshadow -g`) |
| `702memcheck <exe>` | Run Valgrind with full leak checking |
| `702help` | Print the command list |

## Typical workflow

Configure once, then build and run as you work through a week:

```bash
enpm702              # activate the environment
702configure         # first time only, or after adding a new source file
702build             # compile everything
702exe               # see what was built
702run week3_main    # run one program
```

To work on a single week and build only its target:

```bash
702w3                # cd project/week3
702build week3_main
702run week3_main
```

For a quick one-off compile during a lecture, without CMake:

```bash
702w4
702g++ src/example.cpp -o example
./example
702memcheck ./example
```

## Documentation with Doxygen

In `project/week5/arm_demo/`, each function is documented with a Doxygen
comment on its declaration, in the header. To build the reference pages:

```bash
sudo apt install doxygen doxygen-gui graphviz   # once
702w5
cd arm_demo/docs && doxygen Doxyfile
xdg-open html/index.html
```

Run Doxygen **from the folder the `Doxyfile` is in**. Its `INPUT` paths are
relative to the folder Doxygen runs in, not to the file, so
`doxygen docs/Doxyfile` from the week folder finds no source and writes an
empty `html/` in the wrong place.

`doxywizard &` is the same thing with a window. Open `docs/Doxyfile` in it and
it sets the working directory for you.

## Notes

- The standard is C++20, both in the CMake build and in the `702g++` shortcut.
  Keep `CMAKE_CXX_STANDARD` in the top-level `CMakeLists.txt` and `ENPM702_STD`
  in `enpm702-cpp.sh` in sync if you ever change it.
- `702configure` also writes `compile_commands.json` into `build/`, which is what
  clangd and the VS Code C++ extension use for autocompletion and diagnostics.
- Target names such as `week3_main` above are examples. Use the names defined by
  `add_executable` in each week's `CMakeLists.txt`, or run `702exe` to list them.