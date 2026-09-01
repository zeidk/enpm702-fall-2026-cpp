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
│   └── ... week9/
├── enpm702-cpp.sh
├── LICENSE
└── README.md
```

The `build/` directory is created by `702configure` and is git-ignored.

## Setup

### 1. Clone the repository

```bash
git clone https://github.com/zeidk/enpm702-fall-2026-cpp.git ~/enpm702_cpp/enpm702-fall-2026-cpp
```

The path matters: the shell script expects the repository at
`~/enpm702_cpp/enpm702-fall-2026-cpp`. If you clone it somewhere else, edit
`ENPM702_WS` at the top of `enpm702-cpp.sh`.

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

Then restart your terminal or run `source ~/.bashrc` (or `source ~/.zshrc`).

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
| `702g++ <file.cpp>` | Compile a single file (`g++ -std=c++20 -Wall -Wextra -Wpedantic -g`) |
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

## Notes

- The standard is C++20, both in the CMake build and in the `702g++` shortcut.
  Keep `CMAKE_CXX_STANDARD` in the top-level `CMakeLists.txt` and `ENPM702_STD`
  in `enpm702-cpp.sh` in sync if you ever change it.
- `702configure` also writes `compile_commands.json` into `build/`, which is what
  clangd and the VS Code C++ extension use for autocompletion and diagnostics.
- Target names such as `week3_main` above are examples. Use the names defined by
  `add_executable` in each week's `CMakeLists.txt`, or run `702exe` to list them.