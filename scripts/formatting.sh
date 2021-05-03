#!/usr/bin/env bash

echo "Linting your code..."

# shellcheck disable=SC2044
for file in $(find . -iname '*.cpp' -or -iname '*.c' -or -iname '*.h' -or -iname '*.hpp' -and -not -iname 'doctest.h' ); do diff -u <(cat "$file") <(clang-format "$file") || exit 1; done