#!/usr/bin/env bash

set -o pipefail

# Find every test dir that has cmake
TEST_DIRS=$(find . -type d -name test -exec test -f "{}/CMakeLists.txt" \; -print)

if [ -z "$TEST_DIRS" ]; then
    echo "No test found."
    exit 0
fi

for test_dir in $TEST_DIRS; do
    echo "Running tests in: $test_dir"
    build_dir="$test_dir/build"
    mkdir -p "$build_dir"
    cd "$build_dir"
    cmake ..
    make -j
    ctest -V
    cd - > /dev/null
done
