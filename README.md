# ENPM702 - Fall 2026 - C++

C++ code snippets from lecture slides.

## Setup

### 1. Clone the repository

```bash
git clone https://github.com/zeidk/enpm702-fall-2026-cpp.git ~/enpm702-cpp
```

### 2. Source the shell script

Add the following line to your shell configuration file:

**Bash users:**

```bash
echo "source ~/enpm702-cpp/enpm702-cpp.sh" >> ~/.bashrc
```

**Zsh users:**

```bash
echo "source ~/enpm702-cpp/enpm702-cpp.sh" >> ~/.zshrc
```

Then restart your terminal or run `source ~/.bashrc` (or `source ~/.zshrc`).

### 3. Activate the environment

```bash
enpm702
```

This loads navigation aliases, build shortcuts, and lecture helpers into your shell.

## Available Aliases

| Alias | Description |
|---|---|
| `702ws` | Navigate to workspace root |
| `702src` | Navigate to `src/` |
| `702build` | Navigate to `build/` |
| `702l1` ... `702l9` | Navigate to a lecture folder |
| `702configure` | Run CMake (Debug) |
| `702release` | Run CMake (Release) |
| `702build` | Build the project |
| `702clean` | Remove the build directory |
| `702rebuild` | Clean + configure + build |
| `702g++` | Compile a single file (`g++ -std=c++17 -Wall -Wextra -Wpedantic -g`) |
| `702memcheck` | Run Valgrind with full leak checking |
