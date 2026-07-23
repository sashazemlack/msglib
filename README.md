# msglib

Cross-platform C++ shared library that shows a modal message box on **Windows** and **Linux**.

| Platform | Backend |
|----------|---------|
| Windows  | `MessageBoxW` (`user32`) |
| Linux    | GTK 3 dialog (system-installed; not bundled) |

Built for **dynamic linking** (`msglib.dll` / `libmsglib.so`).

## API

Public header: [`include/msglib.h`](include/msglib.h)

```c
int MsgBox(int icon, const char* message, const char* title);
```

| Argument  | Description |
|-----------|-------------|
| `icon`    | `0` = none, `1` = information, `2` = warning, `3` = error |
| `message` | UTF-8 message text (must not be `NULL`) |
| `title`   | UTF-8 window title (must not be `NULL`) |

**Returns:** `1` if the user dismissed the dialog with OK, `0` on failure.

The function uses C linkage (`extern "C"`) so it can be loaded from C, C++, or other languages via dynamic linking.

### Example usage

```c
#include "msglib.h"

int main(void)
{
    MsgBox(2, "Disk space is low.", "Warning");
    return 0;
}
```

## Requirements

### Linux

- CMake 3.16+
- C++17 compiler
- GTK 3 development package (runtime GTK 3 must also be present on target systems)

```bash
# Arch
sudo pacman -S gtk3

# Debian / Ubuntu
sudo apt install libgtk-3-dev
```

### Windows (native)

- CMake 3.16+
- MSVC or MinGW-w64 with C++17

### Windows (cross-compile from Linux)

- `x86_64-w64-mingw32-g++` and/or `i686-w64-mingw32-g++`

```bash
# Arch
sudo pacman -S mingw-w64-gcc

# Debian / Ubuntu
sudo apt install g++-mingw-w64
```

MinGW builds statically link `libgcc`, `libstdc++`, and `winpthread`, so no extra MinGW runtime DLLs are required next to `msglib.dll`.

## Build

### Linux

```bash
cmake -S . -B build
cmake --build build
```

Output: `build/libmsglib.so`

With the example program:

```bash
cmake -S . -B build -DMSGLIB_BUILD_EXAMPLE=ON
cmake --build build
```

### Windows (native)

```bash
cmake -S . -B build -DMSGLIB_BUILD_EXAMPLE=ON
cmake --build build
```

Output: `build/bin/msglib.dll` (and `msglib_example.exe` if the example is enabled)

### Cross-compile for Windows from Linux

**64-bit:**

```bash
cmake -S . -B build-win64 \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw-w64-x86_64.cmake \
  -DMSGLIB_BUILD_EXAMPLE=ON
cmake --build build-win64
```

**32-bit:**

```bash
cmake -S . -B build-win32 \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw-w64-i686.cmake \
  -DMSGLIB_BUILD_EXAMPLE=ON
cmake --build build-win32
```

Outputs are under `build-win64/bin/` or `build-win32/bin/` (`msglib.dll`, optional `msglib_example.exe`).

### Install (optional)

```bash
cmake --install build --prefix /usr/local
```

## Example program

When built with `-DMSGLIB_BUILD_EXAMPLE=ON`, arguments match `MsgBox` directly:

```bash
msglib_example <icon> <message> <title>

# Linux
./build/msglib_example 2 "Disk space is low" "Warning"

# Windows (native or cross-built)
build-win64\bin\msglib_example.exe 1 "Hello" "msglib"
```

## Linking against msglib

### Linux

```bash
g++ main.cpp -I/path/to/msglib/include -L/path/to/msglib/build -lmsglib -Wl,-rpath,/path/to/msglib/build
```

Ensure `libgtk-3.so` is available at runtime (normally provided by the distro).

### Windows

Link against `msglib.dll` (and its import library, e.g. `msglib.lib` / `libmsglib.dll.a`) and keep `msglib.dll` beside your executable or on `PATH`.

```bash
# MinGW example
x86_64-w64-mingw32-g++ main.cpp -Iinclude -Lbuild-win64/lib -lmsglib -o main.exe
```

## Project layout

```
include/msglib.h                          Public API
src/msglib.cpp                            Windows / Linux implementation
examples/example.cpp                      Optional demo CLI
cmake/toolchains/mingw-w64-x86_64.cmake   MinGW 64-bit toolchain
cmake/toolchains/mingw-w64-i686.cmake     MinGW 32-bit toolchain
CMakeLists.txt
```
