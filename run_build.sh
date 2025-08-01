#!/usr/bin/env bash

if [[ "$#" -ne 1 || ( "$1" != "good" && "$1" != "bad" ) ]]; then
    echo "Error: You must specify 'good' or 'bad'"
    echo "Usage: ./build.sh [good|bad]"
    exit 1
fi

MODE=$1

VENV=".venv_$MODE"
REQUIREMENTS="requirements_${MODE}.txt"
CONAN_HOME_DIR=".conan2_$MODE"
BUILD_FOLDER="build_$MODE"
TEST_FOLDER="test_$MODE"

python3 -m venv "$VENV"
source "$VENV/bin/activate"

pip3 install -r "$REQUIREMENTS"

export CONAN_HOME=$(pwd)/"$CONAN_HOME_DIR"

conan config home
conan config install ./conan_config/
conan profile detect

conan build . \
    -s build_type=Debug \
    -c tools.cmake.cmake_layout:build_folder="$BUILD_FOLDER" \
    -c tools.cmake.cmake_layout:test_folder="$TEST_FOLDER" \
    --build missing
