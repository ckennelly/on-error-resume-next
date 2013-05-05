CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Warnings
SET(WARNINGS "-Wall -Wextra -Wpedantic -Wshadow -Wnested-externs -Wstrict-prototypes -Wno-int-to-pointer-cast -Werror")
SET(CMAKE_C_FLAGS "${WARNINGS} -std=c99 -D_POSIX_C_SOURCE=200809L")

# Build targets
ENABLE_TESTING()
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(samples)
