// Redirect to board-level pins_arduino.h
// This file exists so the variant directory contains NO .cpp files,
// preventing pioarduino-build.py from trying to compile interface.cpp
// with limited include paths (no LDF library headers available yet).
// interface.cpp is compiled separately via build_src_filter.
#include "../pins_arduino.h"
