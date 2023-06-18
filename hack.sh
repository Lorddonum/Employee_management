#!/bin/sh

set -xe

zig cc -std=c2x -pedantic -Wall -Wextra -g3 -ggdb src/*.c libs/static/libraylib.a -o emp.out && ./emp.out
