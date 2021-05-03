#!/usr/bin/env bash

echo "Linting your code..."

# shellcheck disable=SC2044
for file in $(find ./src -iname '*.cpp' -or -iname '*.c' -or -iname '*.h' -and -not -iname 'doctest.h' ); do clang-format -style=google -i "$file" ; done