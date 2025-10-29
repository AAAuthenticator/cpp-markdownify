#!/bin/bash

set -e

echo "Building markdownify-cpp..."

mkdir -p build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo ""
echo "Build complete! Binaries are in the build directory."
echo ""
echo "To run the example:"
echo "  ./build/examples/basic_usage"
echo ""
echo "To run tests:"
echo "  cd build && ctest --verbose"
echo ""