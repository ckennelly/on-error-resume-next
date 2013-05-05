FILE(GLOB launcher_SOURCES *.c)

ADD_EXECUTABLE(launcher ${launcher_SOURCES})
SET_TARGET_PROPERTIES(launcher PROPERTIES OUTPUT_NAME on-error-resume-next)
