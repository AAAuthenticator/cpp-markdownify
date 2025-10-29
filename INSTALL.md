# Installation Guide

## Quick Start

```bash
# Build the library
./build.sh

# Run the example
./build/examples/basic_usage

# Run tests
cd build && ctest --verbose
```

Expected output:
```
100% tests passed, 0 tests failed out of 31
Total Test time (real) =   0.06 sec
```

## Manual Build

### Prerequisites

- CMake 3.14+
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git (for fetching dependencies)

### Build Steps

```bash
# Create and enter build directory
mkdir build && cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . -j$(nproc)

# Test (optional)
ctest --output-on-failure

# Install system-wide (optional)
sudo cmake --install .
```

## Build Options

Configure the build with these CMake options:

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DMARKDOWNIFY_BUILD_EXAMPLES=ON \
  -DMARKDOWNIFY_BUILD_TESTS=ON \
  -DMARKDOWNIFY_USE_SYSTEM_TINYXML2=OFF
```

### Available Options

- `CMAKE_BUILD_TYPE`: Build type (Debug, Release, RelWithDebInfo, MinSizeRel)
- `MARKDOWNIFY_BUILD_EXAMPLES`: Build example programs (default: ON)
- `MARKDOWNIFY_BUILD_TESTS`: Build test suite (default: ON)
- `MARKDOWNIFY_USE_SYSTEM_TINYXML2`: Use system TinyXML2 instead of fetching (default: OFF)

## Installing to Custom Location

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
cmake --build .
cmake --install .
```

## Using in Your Project

### Method 1: CMake FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
    markdownify
    GIT_REPOSITORY <repository-url>
    GIT_TAG main
)

FetchContent_MakeAvailable(markdownify)

target_link_libraries(your_target PRIVATE markdownify)
```

### Method 2: Installed Library

After installation:

```cmake
find_package(markdownify REQUIRED)
target_link_libraries(your_target PRIVATE markdownify::markdownify)
```

### Method 3: Add as Subdirectory

```cmake
add_subdirectory(path/to/cpp-markdownify)
target_link_libraries(your_target PRIVATE markdownify)
```

## Platform-Specific Notes

### Linux

```bash
# Install build tools
sudo apt-get install build-essential cmake git

# Build
./build.sh
```

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Or use Homebrew
brew install cmake

# Build
./build.sh
```

### Windows

Using Visual Studio:

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

Or with MinGW:

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

## Troubleshooting

### TinyXML2 Not Found

If you want to use system TinyXML2:

```bash
# Linux
sudo apt-get install libtinyxml2-dev

# macOS
brew install tinyxml2

# Then build with
cmake .. -DMARKDOWNIFY_USE_SYSTEM_TINYXML2=ON
```

### Compiler Version Too Old

Ensure your compiler supports C++17:

```bash
# Check GCC version (need 7+)
g++ --version

# Check Clang version (need 5+)
clang++ --version
```

### CMake Version Too Old

```bash
# Upgrade CMake on Ubuntu
sudo apt-get update
sudo apt-get install cmake

# Or download latest from cmake.org
```

## Verifying Installation

After installation, verify it works:

```cpp
// test.cpp
#include <markdownify/markdownify.hpp>
#include <iostream>

int main() {
    std::string html = "<b>Test</b>";
    std::cout << markdownify::markdownify(html) << std::endl;
    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 test.cpp -lmarkdownify -ltinyxml2 -o test
./test
```

Expected output: `**Test**`

## Testing More Features

Test various HTML tags:

```cpp
#include <markdownify/markdownify.hpp>
#include <iostream>

int main() {
    // Test different conversions
    std::cout << "Bold: " << markdownify::markdownify("<b>Bold</b>") << std::endl;
    std::cout << "Italic: " << markdownify::markdownify("<em>Italic</em>") << std::endl;
    std::cout << "Link: " << markdownify::markdownify("<a href='http://example.com'>Link</a>") << std::endl;
    std::cout << "Image: " << markdownify::markdownify("<img src='test.png' alt='Test'>") << std::endl;
    std::cout << "HR: " << markdownify::markdownify("<hr>") << std::endl;

    // Test with options
    markdownify::Options opts;
    opts.heading_style = markdownify::HeadingStyle::ATX;
    std::cout << "Heading: " << markdownify::markdownify("<h1>Title</h1>", opts) << std::endl;

    return 0;
}
```

Expected output:
```
Bold: **Bold**
Italic: *Italic*
Link: [Link](http://example.com)
Image: ![Test](test.png)
HR: ---
Heading: # Title
```

## Build Verification

Run the full test suite to verify everything works:

```bash
cd build
ctest --verbose
```

All 31 tests should pass:
- 18 basic functionality tests (text formatting, links, images, lists, etc.)
- 13 advanced conversion tests (options, filtering, escaping, complex documents)

If any tests fail, please check:
1. C++17 compiler is being used
2. TinyXML2 was fetched correctly
3. All source files compiled without errors